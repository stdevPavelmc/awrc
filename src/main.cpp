// code exclusive for ESP8266
#if !(defined(ESP8266))
#error This code is intended to run on ESP8266 platform! Please check your Tools->Board setting.
#endif

// libraries
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <AsyncElegantOTA.h>
#include <ESP8266WiFiMulti.h>
#include <ArduinoOTA.h>
#include <DNSServer.h>
#include <vector>

#include "config.h"

ESP8266WiFiMulti wifiMulti;

static DNSServer DNS;
AsyncWebServer webServer(80);
static std::vector<AsyncClient *> clients; // a list to hold all clients

/****************************************************/

// Wifi default credentials for the softAP if no wifi
const char *ssid = APSSID;
const char *password = APPSK;

// position variables

// actual values
float azimuth = 0.0;
float elevation = 0.0;
// target values
float tazimuth = 0.0;
float televation = 0.0;
// minimum error to track
float minerror = 5.0;

// pulse count vars
int azpulsecount = 0;
int elpulsecount = 0;
volatile sint8 azpulses = 0;
volatile sint8 elpulses = 0;

// direction (yes volatile as it will be use on a ISR routine)
volatile sint8 azdir = 0; //  1 = right / 0 = stoped / -1 = left
volatile sint8 eldir = 0; //  1 = up / 0 = stoped / -1 = down

// pulses/degrees ratio
float azdratio = 43.055555556; // tentative value
float eldratio = 21.527777778; // tentative value

// limits flags
volatile bool azlimit = 0;
volatile bool ellimit = 0;

/*****************************
 * Miscelaneous functions
******************************/

// split a string in parts by a given separator
String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = {0, -1};
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++)
    {
        if (data.charAt(i) == separator || i == maxIndex)
        {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i + 1 : i;
        }
    }

    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

/*********************************
 * Basic movements of the motors
**********************************/

// stop azimuth movement
void az_stop()
{
    // reset target = actual
    tazimuth = azimuth;

    // full motor stop
    digitalWrite(MRIGHT, LOW);
    digitalWrite(MLEFT, LOW);

    // induced pause
    delay(1000);

    // dir update
    azdir = 0;

    // debug
#ifdef DEBUG
    Serial.println("AZ STOP");
#endif
}

// stop elevation movement
void el_stop()
{
    // reset target = actual
    televation = elevation;

    // full motor stop
    digitalWrite(MUP, LOW);
    digitalWrite(MDOWN, LOW);

    // induced pause
    delay(1000);

    // dir update
    eldir = 0;

// debug
#ifdef DEBUG
    Serial.println("EL STOP");
#endif
}

// move right
void move_right()
{
    // if already moving do nothing
    if (digitalRead(MRIGHT))
        return;

    // if was moving on the other direction stop & pause before move
    if (digitalRead(MLEFT))
        az_stop();

    // move
    digitalWrite(MRIGHT, HIGH);

    // dir update
    azdir = 1;

// debug
#ifdef DEBUG
    Serial.println("MOVE RIGHT");
#endif
}

// move left
void move_left()
{
    // if already moving do nothing
    if (digitalRead(MLEFT))
        return;

    // if was moving on the other direction stop & pause before move
    if (digitalRead(MRIGHT))
        az_stop();

    // move
    digitalWrite(MLEFT, HIGH);

    // dir update
    azdir = -1;

// debug
#ifdef DEBUG
    Serial.println("MOVE LEFT");
#endif
}

// move up
void move_up()
{
    // if already moving do nothing
    if (digitalRead(MUP))
        return;

    // if was moving on the other direction stop & pause before move
    if (digitalRead(MDOWN))
        el_stop();

    // move
    digitalWrite(MUP, HIGH);

    // dir update
    eldir = 1;

// debug
#ifdef DEBUG
    Serial.println("MOVE UP");
#endif
}

// move down
void move_down()
{
    // if already moving do nothing
    if (digitalRead(MDOWN))
        return;

    // if was moving on the other direction stop & pause before move
    if (digitalRead(MUP))
        el_stop();

    // move
    digitalWrite(MDOWN, HIGH);

    // dir update
    eldir = -1;

    // debug
#ifdef DEBUG
    Serial.println("MOVE DOWN");
#endif
}

/************************************************
 *  Command interface functions
*************************************************/

// set position
void set_position(String data)
{
    // replace "," by "."
    data.replace(",", ".");

    // extract numbers
    float a = getValue(data, 32, 1).toFloat();
    float e = getValue(data, 32, 2).toFloat();

    // numbers limits
    if (a > 360)
        a = 360;
    if (a < 0)
        a = 0;
    if (e > 90)
        e = 90;
    if (e < 0)
        e = 0;

    // finaly set them on the env
    tazimuth = a;
    televation = e;

    // debug
#ifdef DEBUG
    Serial.print("P ");
    Serial.print(tazimuth);
    Serial.print(" ");
    Serial.println(televation);
#endif
}

// get position
String get_position()
{
    Serial.println("p");
    char az[8];
    char el[8];
    dtostrf(azimuth, 3, 2, az);
    dtostrf(elevation, 3, 2, el);
    String azim = String(az);
    String elev = String(el);
    String result = azim + String("\n") + elev;
    result.replace(".", ",");
    return result;
}

// full stop
void full_stop()
{
#ifdef DEBUG
    Serial.println("S");
#endif

    // motors
    // az
    digitalWrite(MRIGHT, LOW);
    digitalWrite(MLEFT, LOW);
    // el
    digitalWrite(MUP, LOW);
    digitalWrite(MDOWN, LOW);

    // dirs
    azdir = 0;
    eldir = 0;

    // target = actual
    tazimuth = azimuth;
    televation = elevation;

    // debug
#ifdef DEBUG
    Serial.println("FULL STOP");
#endif
}

// parking
void parking()
{
    Serial.println("K");
}

// move to speed
void move_to(String data)
{
    Serial.println("M");
}

// handle the client messages
String msg_handle(char *data)
{
    String d = String(data);
    String result = String("RPRT 0");
    char command = d.charAt(0);

    switch (command)
    {
    // set position
    case 80: // P
        set_position(d);
        break;
    // request position
    case 112: // p
        result = get_position();
        break;
    // stop
    case 83: // S
        full_stop();
        break;
    // parking
    case 75: // K
        parking();
        break;
    // move to position
    case 77: // M
        move_to(d);
        break;

    default:
        break;
    }

// debug
#ifdef DEBUG
    Serial.println(result);
#endif

    // client answer
    return result;
}

/*******************
 * TCP socket part
 *******************/

static void handleError(void *arg, AsyncClient *client, int8_t error)
{
    Serial.printf("\n connection error %s from client %s \n", client->errorToString(error), client->remoteIP().toString().c_str());
}

static void handleData(void *arg, AsyncClient *client, void *data, size_t len)
{
    // prepare buffers
    char buff[18];
    memset(buff, '\0', 18);
    char reply[32];
    memset(reply, '\0', 32);

    // copy only the starting 18 chars of the payload 
    memcpy(buff, data, len);

    // send the data to the handle function
    String answer = msg_handle(buff);

    // reply to client
    if (client->space() > 32 && client->canSend())
    {
        memcpy(reply, answer.c_str(), strlen(answer.c_str()));
        client->add(reply, strlen(reply));
        client->send();
    }
}

static void handleDisconnect(void *arg, AsyncClient *client)
{
    Serial.printf("\n client %s disconnected \n", client->remoteIP().toString().c_str());
}

static void handleTimeOut(void *arg, AsyncClient *client, uint32_t time)
{
    Serial.printf("\n client ACK timeout ip: %s \n", client->remoteIP().toString().c_str());
}

static void handleNewClient(void *arg, AsyncClient *client)
{
    Serial.printf("\n new client has been connected to server, ip: %s\n", client->remoteIP().toString().c_str());

    // add to list
    clients.push_back(client);

    // register events
    client->onData(&handleData, NULL);
    client->onError(&handleError, NULL);
    client->onDisconnect(&handleDisconnect, NULL);
    client->onTimeout(&handleTimeOut, NULL);
}

/************************************
 * Position calculations and checks
*************************************/

// calc position
void calc_position()
{
    azimuth = azpulsecount / azdratio;
    elevation = elpulsecount / eldratio;

    // failsafe
    if (azimuth > MAXAZIMUTH)
        azimuth = MAXAZIMUTH;
    if (azimuth < MINAZIMUTH)
        azimuth = MINAZIMUTH;
    // failsafe
    if (elevation > MAXELEVATION)
        azimuth = MAXELEVATION;
    if (azimuth < MINELEVATION)
        azimuth = MINELEVATION;

    // limits case
    if (azlimit == 1)
    { // azimuth limits
        if (digitalRead(MRIGHT) == 1)
            azimuth = MAXAZIMUTH;
        if (digitalRead(MLEFT) == 1)
            azimuth = MINAZIMUTH;
    }
    if (ellimit == 1)
    {
        if (digitalRead(MUP) == 1)
            elevation = MAXELEVATION;
        if (digitalRead(MDOWN) == 1)
            elevation = MINELEVATION;
    }
}

// update the real positions from pulses
void update_position()
{
    // disable interrupt
    noInterrupts();

    // sum azimuth
    if (azpulses !=0)
    {
        azimuth += azpulses;
        azpulses = 0;
    }

    // sum elevation
    if (elpulses != 0)
    {
        elevation += elpulses;
        elpulses = 0;
    }

    // enable interrupts
    interrupts();

    // calc real pointing position
    calc_position();
}

// we need to move on az?
void need2move_az(float delta)
{
    // old movement
    sint8 oldazdir = azdir;

    // limits hit: STOP!
    // if (azlimit == 1)
    //     az_stop();

    // need to move
    if (abs(delta) > minerror)
    {
        // debug
#ifdef DEBUG
        Serial.print("Az diff: ");
        Serial.println(delta);
#endif
      
        // ok, we need to move if no limits are hit
        if ((oldazdir != 1) and (delta > 0))
        {
            // move rigth
            move_right();
        }
        if ((oldazdir != -1) and (delta < 0))
        {
            // move left
            move_left();
        }
    }
    else
    {
        // no need to move stop motors
        az_stop();
    }
}

// we need to move on el?
void need2move_el(float delta)
{
    // old movement
    sint8 oldeldir = eldir;

    // limits hit: full STOP
    // if (ellimit == 1)
    //     el_stop();

    // need to move
    if (abs(delta) > minerror)
    {
        // debug
#ifdef DEBUG
        Serial.print("El diff: ");
        Serial.println(delta);
#endif

        // ok, we need to move if no limits are hit
        if ((oldeldir != 1) and (delta > 0))
        {
            // move up
            move_up();
        }
        if ((oldeldir != -1) and (delta < 0))
        {
            // move down
            move_down();
        }
    }
    else
    {
        // no need to move stop motors
        el_stop();
    }
}

// check if we need to move and activate the corresponding motors
void  need2move()
{
    // azimuth difference
    float daz = tazimuth - azimuth;
    // move if needed
    need2move_az(daz);

    // elevation
    float del = televation - elevation;
    // move if needed
    need2move_el(del);
}

/**********************
 * Setup funtions
***********************/

// pin mode declaration
void pin_setup()
{
    // movement outputs
    pinMode(MRIGHT, OUTPUT);
    pinMode(MLEFT, OUTPUT);
    pinMode(MDOWN, OUTPUT);
    pinMode(MUP, OUTPUT);
    digitalWrite(MRIGHT, LOW);
    digitalWrite(MLEFT, LOW);
    digitalWrite(MUP, LOW);
    digitalWrite(MDOWN, LOW);

    // Input movement interrupts
    pinMode(IIAZ, INPUT_PULLUP);
    pinMode(IIEL, INPUT_PULLUP);

    // movement current sensor
    pinMode(AZLIMIT, INPUT_PULLUP);
    pinMode(ELLIMIT, INPUT_PULLUP);

    // Rotor transformer relay
    pinMode(TRAFOON, OUTPUT);
    digitalWrite(TRAFOON, LOW);
}

// wifi config
void wifi_config()
{
    WiFi.mode(WIFI_STA);
    wifiMulti.addAP("pavel", "1234567890a");
    wifiMulti.addAP("CO7WT", "Xilantr0!!!");

    Serial.println("Connecting to Wifi...");
    if (wifiMulti.run() == WL_CONNECTED)
    {
        Serial.println("");
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
    }
    else
    {
        Serial.println("No connection detected...");
    }
    
}

// tcp socket to sumulate a rotctld
void rotctld_setup()
{
    AsyncServer *tcpserver = new AsyncServer(TCP_PORT); // start listening on tcp port 7050
    tcpserver->onClient(&handleNewClient, tcpserver);
    tcpserver->begin();
}

// ota setup
void ota_setup()
{
    ArduinoOTA.onStart([]() {
        String type;

        if (ArduinoOTA.getCommand() == U_FLASH)
        {

            type = "sketch";
        }
        else
        {
            // U_FS

            type = "filesystem";
        }

        // NOTE: if updating FS this would be the place to unmount FS using FS.end()

        Serial.println("Start updating " + type);
    });

    ArduinoOTA.onEnd([]() {
        Serial.println("\nEnd");
    });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });

    ArduinoOTA.onError([](ota_error_t error) {

		Serial.printf("Error[%u]: ", error);

		if (error == OTA_AUTH_ERROR)
		{

			Serial.println("Auth Failed");
		}
		else if (error == OTA_BEGIN_ERROR)
		{

			Serial.println("Begin Failed");
		}
		else if (error == OTA_CONNECT_ERROR)
		{

			Serial.println("Connect Failed");
		}
		else if (error == OTA_RECEIVE_ERROR)
		{

			Serial.println("Receive Failed");
		}
		else if (error == OTA_END_ERROR)
		{

			Serial.println("End Failed");
		} });

    ArduinoOTA.begin();
}

/*********************
 * Interrupts part
**********************/

// rotation azimuth interrupts
void IRAM_ATTR azinterrupt()
{
    if (azdir > 0)
        azpulses += 1;
    if (azdir < 0)
        azpulses -= 1;
}

// rotation elevation interrupt
void IRAM_ATTR elinterrupt()
{
    if (eldir > 0)
        elpulses += 1;
    if (eldir < 0)
        elpulses -= 1;
}

// az limit hit
void IRAM_ATTR azlimhit()
{
    azlimit = !digitalRead(AZLIMIT);
}

// el limit hit
void IRAM_ATTR ellimhit()
{
    ellimit = !digitalRead(ELLIMIT);
}

// interrupts setup
void interrupt_setup()
{
    // az/el movement
    attachInterrupt(digitalPinToInterrupt(IIAZ), azinterrupt, CHANGE);
    attachInterrupt(digitalPinToInterrupt(IIEL), elinterrupt, CHANGE);

    // limits interrupts
    attachInterrupt(digitalPinToInterrupt(AZLIMIT), azlimhit, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ELLIMIT), ellimhit, CHANGE);
}

/****************************
 * Web server routines
******************************/

// setup the web server
void webserver_setup()
{
    webServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", "Hi! I am ESP8266.");
    });

    // OTA settings
    AsyncElegantOTA.begin(&webServer, "admin", "Mi rotor!"); // Start ElegantOTA
    webServer.begin();
    Serial.println("HTTP server started");
}

/***********************************************************************************/

// setup
void setup()
{
    Serial.begin(115200);
    delay(200);

    Serial.println("\n\nArduino Wireless rotor controller v0.1 booting...\n");

    // wifi config
    wifi_config();

    // TCP socket to simulate a rigctld
    rotctld_setup();

    //pin declaration
    pin_setup();

    // interrupt declaration
    interrupt_setup();

    // setup web server
    webserver_setup();

    // OTA setup
    ota_setup();

    // free sketch space
    Serial.print("\nFree space: ");
    Serial.println(ESP.getFreeSketchSpace());
}

// main loop
void loop()
{
    // check if the position changed
    update_position();

    // need to move? (always AFTER the update_position one)
    need2move();

    // multiwifi run
    wifiMulti.run();

    // ota handler
    ArduinoOTA.handle();
}
