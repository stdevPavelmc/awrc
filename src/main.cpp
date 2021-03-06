// code exclusive for ESP8266
#if !(defined(ESP8266))
#error This code is intended to run on ESP8266 platform! Please check your Tools->Board setting.
#endif

// libraries
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP8266WiFiMulti.h>
#include <ArduinoOTA.h>
#include <vector>
#include <arduino-timer.h>
#include "LittleFS.h"

// own / local
#include "eeprom.h"
#include "config.h"

// instantiating
Config conf;
States state = IDLE;
WebCmds wcmd = NONE;
ESP8266WiFiMulti wifiMulti;
//static DNSServer DNS;
AsyncWebServer webServer(80);
static std::vector<AsyncClient *> clients;
auto timer = timer_create_default(); // create a timer with default settings

/****************************************************/

// position variables

// actual values
float azimuth = 0.0;
float elevation = 0.0;
// target values
float tazimuth = 0.0;
float televation = 0.0;

// inertia on axes
float azinertia = 0;
float elinertia = 0;
// last time we moved on an axis
unsigned long aztime = 0;
unsigned long eltime = 0;
// stopped flags
bool azstopped = true;
bool elstopped = true;
// position to stop command
float azstoppos = 0;
float elstoppos = 0;

// pulse count vars
int azpulsecount = 0;
int elpulsecount = 0;
volatile sint8 azpulses = 0;
volatile sint8 elpulses = 0;

// direction (yes volatile as it will be use on a ISR routine)
volatile sint8 azdir = 0; //  1 = right / 0 = stoped / -1 = left
volatile sint8 eldir = 0; //  1 = up / 0 = stoped / -1 = down

// pulses/degrees ratio
float azdratio = DEF_AZDRATIO; // tentative value (see config.h)
float eldratio = DEF_ELDRATIO; // tentative value (see config.h)

// prototypes
void calc_position();

// SOME progmem

const char pstatus[] PROGMEM = "{\"az\": \"%az%\", \"el\": \"%el%\", \"taz\": \"%taz%\", \"tel\": \"%tel%\", \"status\": \"%st%\", \"rssi\": \"%rs%\"}";

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
    bool mod = false;

    // full motor stop
    if (digitalRead(MRIGHT))
    {
        digitalWrite(MRIGHT, LOW);
        mod = true;
    }
    if (digitalRead(MLEFT))
    {
        digitalWrite(MLEFT, LOW);
        mod = true;
    }

    // reset only if neede
    if (mod)
    {
        // postition of the stop command
        azstoppos = azimuth;

        // induced pause
        delay(200);

        // dir update
        azdir = 0;

        // debug
#ifdef DEBUG
        Serial.println("AZ STOP");
#endif
    }
}

// stop elevation movement
void el_stop()
{
    bool mod = false;

    // full motor stop
    if (digitalRead(MUP))
    {
        digitalWrite(MUP, LOW);
        mod = true;
    }
    if (digitalRead(MDOWN))
    {
        digitalWrite(MDOWN, LOW);
        mod = true;
    }

    // reset only if neede
    if (mod)
    {
        // postition of the stop command
        elstoppos = elevation;

        // induced pause
        delay(200);

        // dir update
        eldir = 0;

        // debug
#ifdef DEBUG
        Serial.println("EL STOP");
#endif
    }
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
    if (a > MAXAZIMUTH)
        a = MAXAZIMUTH;
    if (a < MINAZIMUTH)
        a = MINAZIMUTH;
    if (e > MAXELEVATION)
        e = MAXELEVATION;
    if (e < MINELEVATION)
        e = MINELEVATION;

    // finaly set them on the env as target
    tazimuth = a;
    televation = e;

    // declare status
    state = TRACKING;

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
    Serial.print("azp/az: ");
    Serial.print(azpulsecount);
    Serial.print("/");
    Serial.print(azimuth);
    Serial.print(" elp/el: ");
    Serial.print(elpulsecount);
    Serial.print("/");
    Serial.println(elevation);

    // build the response string
    char az[8];
    char el[8];
    dtostrf(azimuth, 3, 2, az);
    dtostrf(elevation, 3, 2, el);
    String result = String(az) + String("\n") + String(el) + String("\n");
    result.replace(".", ",");
    return result;
}

// get calibration parameters
String get_calibration()
{
    Serial.print("AZ ratio: ");
    Serial.print(azdratio);
    Serial.print(" | EL ratio: ");
    Serial.println(eldratio);

    // build the response string
    char azd[8];
    char eld[8];
    dtostrf(azdratio, 3, 2, azd);
    dtostrf(eldratio, 3, 2, eld);
    String result = String("AZd: ") + String(azd) + String("\n") + String("ELd: ") + String(eld) + String("\n");
    result.replace(".", ",");
    return result;
}

// full stop
void full_stop()
{
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

    // state
    state = IDLE;

    // debug
#ifdef DEBUG
    Serial.println("FULL STOP");
#endif
}

// parking
void parking()
{
    // move to az = 0 / el = 0 with no error, 0,000 flat!
    tazimuth = 0;
    televation = 0;

    // going to left/down all the way down
    move_left();
    move_down();
    delay(100);
    if (digitalRead(ELLIMIT) or digitalRead(AZLIMIT))
        return;

    // if you reached here then both limits hare hit

    // reset pulsescount
    azpulsecount = 0;
    elpulsecount = 0;
    // reset  position
    azimuth = 0;
    elevation = 0;

    // set state
    state = IDLE;
}

// make a calibration cycle
void calibration()
{
    state = CALIBRATING;

    // going to left/down all the way down
    move_left(); move_down();
    while (digitalRead(ELLIMIT) or digitalRead(AZLIMIT))
    {
        delay(200);
        Serial.print(".");
        calc_position();
    }

    // stop all movement
    full_stop();

    // set state again
    state = CALIBRATING;

    // end the printing
    Serial.println(".");
    Serial.println("Done parking");
    Serial.println("Blocking calibration azimuth...");

    // reset pulses
    azpulsecount = 0;
    elpulsecount = 0;

    // going to left/down all the way down
    move_right();
    move_up();
    while (digitalRead(ELLIMIT) or digitalRead(AZLIMIT))
    {
        delay(200);
        Serial.print(".");
    }

    // end the printing
    Serial.println(".");
    Serial.println("Done");

    // calculation of the ration
    if (azpulsecount > 0)
        azdratio = azpulsecount / (MAXAZIMUTH - MINAZIMUTH);
    else
        azdratio = 1;
    
    if (elpulsecount > 0)
        eldratio = elpulsecount / (MAXELEVATION - MINELEVATION);
    else
        eldratio = 1;
    
    // debug
    Serial.print("AZratio: ");
    Serial.print(azdratio);
    Serial.print("ELratio: ");
    Serial.println(eldratio);

    // save conf
    saveConf(conf);

    // state
    state = IDLE;
}

// '\dump_state' command
String dump_state()
{
    String endc = "\n";
    String result = String("0") + endc;
    result += String("1") + endc;
    result += String(MINAZIMUTH) + endc;
    result += String(MAXAZIMUTH) + endc;
    result += String(MINELEVATION) + endc;
    result += String(MAXELEVATION) + endc;

    return result;
}

// handle the client messages
String msg_handle(char *data)
{
    String d = String(data);
    String result = String("RPRT 0\n");
    char command = d.charAt(0);

    // if calibrating just reply ok and return
    if (state == CALIBRATING)
        return String(result);

    // select the action depending on the command
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
        state = PARKING;
        break;
    // make a calibration cycle
    case 43: // C
        state = CALIBRATING;
        calibration();
        break;
    // retrieve calibration parameters
    case 99: // c
        result = get_calibration();
        break;
    // move RIGHT
    case 114: // r
    case  82: // R
        move_right();
        state = MANUAL;
        break;
    // move LEFT
    case 108: // l
    case  76:  // L
        move_left();
        state = MANUAL;
        break;
    // move UP
    case 117: // u
    case  85:  // U
        move_up();
        state = MANUAL;
        break;
    // move DOWN
    case 100: // d
    case 68:  // D
        move_down();
        state = MANUAL;
        break;

    case 92: // \dump_state
        // call the dump procedure
        result = dump_state();
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
    if (azpulsecount != 0)
    {
        azimuth = float(azpulsecount) / azdratio;
    }
    else
    {
        azimuth = 0;
    }

    if (elpulsecount != 0)
        elevation = float(elpulsecount) / eldratio;
    else
        elevation = 0;

    // failsafe AZ
    if (azimuth > MAXAZIMUTH) {azimuth = MAXAZIMUTH;}
    if (azimuth < MINAZIMUTH) {azimuth = MINAZIMUTH;}
    // failsafe EL
    if (elevation > MAXELEVATION) {elevation = MAXELEVATION;}
    if (elevation < MINELEVATION) {elevation = MINELEVATION;}
}

// update the real positions from pulses
void update_position()
{
    // disable interrupt
    noInterrupts();

    // sum azimuth
    if (azpulses != 0)
    {
        azpulsecount += azpulses;
        azpulses = 0;

        // reset last movement time mark
        aztime = millis();
    }

    // sum elevation
    if (elpulses != 0)
    {
        elpulsecount += elpulses;
        elpulses = 0;

        // reset last movement time mark
        eltime = millis();
    }

    // enable interrupts
    interrupts();

    // update az/el
    calc_position();
}

// we need to move on az?
void need2move_az(float delta)
{
    // manual
    if (state == MANUAL)
        return;

    // absolute value of delta
    float adelta = abs(delta);

    // inertia and i'm moving to make the stop
    if (azinertia != 0 and azdir != 0)
    {
        // moving right and hit inertia window
        if ((azdir == 1) and (delta - azinertia) <= 0)
        {
            az_stop();
            return;
        }

        // moving left and hit inertia window
        if ((azdir == -1) and (delta + azinertia) >= 0)
        {
            az_stop();
            return;
        }
    }

    // from moving left
    if (azdir == -1 and delta >= 0)
    {
        az_stop();
        return;
    }

    // from moving right
    if (azdir == 1 and delta <= 0)
    {
        az_stop();
        return;
    }

    // from stopped position
    if (azdir == 0)
    {
        // mode right
        if (delta > 0 and adelta > MINERROR)
        {
            move_right();
            return;
        }

        // mode left
        if (delta < 0 and adelta > MINERROR)
        {
            move_left();
            return;
        }
    }
}

// we need to move on el?
void need2move_el(float delta)
{
    // manual
    if (state == MANUAL)
        return;

    // absolute value of delta
    float adelta = abs(delta);

    // inertia and i'm moving to make the stop
    if (elinertia != 0 and eldir != 0)
    {
        // moving up and hit inertia window
        if ((eldir == 1) and (delta - elinertia) <= 0)
        {
            el_stop();
            return;
        }

        // moving down and hit inertia window
        if ((eldir == -1) and (delta + elinertia) >= 0)
        {
            el_stop();
            return;
        }
    }

    // from moving down
    if (eldir == -1 and delta >= 0)
    {
        el_stop();
        return;
    }

    // from moving up
    if (eldir == 1 and delta <= 0)
    {
        el_stop();
        return;
    }

    // from stopped position
    if (eldir == 0)
    {
        // move up
        if (delta > 0 and adelta > MINERROR)
        {
            move_up();
            return;
        }

        // move down
        if (delta < 0 and adelta > MINERROR)
        {
            move_down();
            return;
        }
    }
}

// check if we need to move and activate the corresponding motors
void need2move()
{
    // azimuth difference
    float daz = tazimuth - azimuth;
    // move if needed
    need2move_az(daz);

    // elevation difference
    float del = televation - elevation;
    // move if needed
    need2move_el(del);
}

// Limits
void limits()
{
    // azimuth
    if (!digitalRead(AZLIMIT))
    {
        // moving right
        if (azdir > 0)
        {
            azpulsecount = MAXAZIMUTH * azdratio;
            digitalWrite(MRIGHT, LOW);   
        }
        // moving left
        if (azdir < 0)
        {
            azpulsecount = 0;
            digitalWrite(MLEFT, LOW);
        }
        // stoppped
        azdir = 0;
    }

    // elevation
    if (!digitalRead(ELLIMIT))
    {
        // moving UP
        if (eldir > 0)
        {
            elpulsecount = MAXELEVATION * eldratio;
            digitalWrite(MUP, LOW);
        }
        // moving down
        if (eldir < 0)
        {
            elpulsecount = 0;
            digitalWrite(MDOWN, LOW);
        }
        // stoppped
        eldir = 0;
    }
}

// know if moving and calc the inertia of the axis
void ismoving()
{
    // az
    unsigned long delta = millis() - aztime;
    if (delta > AZSTOPT)
    {
        azstopped = true;

        // calc inertia only if tracking
        if (state == TRACKING)
        {
            if (azinertia == 0)
                azinertia = abs(azimuth - azstoppos);
            else
                azinertia = (azinertia + abs(azimuth - azstoppos)) / 2;

        }
    }
    else
        azstopped = false;

    // el
    delta = millis() - eltime;
    if (delta > ELSTOPT)
    {
        elstopped = true;

        // calc inertia only if tracking
        if (state == TRACKING)
        {
            if (elinertia == 0)
                elinertia = abs(elevation - elstoppos);
            else
                elinertia = (elinertia + abs(elevation - elstoppos)) / 2;
        }
    }
    else
        elstopped = false;
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
    pinMode(IIAZ, INPUT);
    pinMode(IIEL, INPUT);

    // movement current sensor
    pinMode(AZLIMIT, INPUT);
    pinMode(ELLIMIT, INPUT);
}

// wifi config
void wifi_config()
{
    // set hostname
    WiFi.hostname(HOSTNAME);

    // start wifi handling
    WiFi.mode(WIFI_STA);
    wifiMulti.addAP("termopilas", "QF9vhtwj");
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

    // register mDNS and names
    if (!MDNS.begin(HOSTNAME))
    {
        Serial.println("Error setting up MDNS responder!");
    }
    else
    {
        MDNS.addService("rotctld", "tcp", TCP_PORT);
        MDNS.addService("http", "tcp", 80);
        Serial.print("mDSN name registered ");
        Serial.print(HOSTNAME);
        Serial.println(".local");
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

        // state
        state = OTA;

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

// timed save
bool confSave(void *)
{
    saveConf(conf);
    return true; // repeat it?
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

// interrupts setup
void interrupt_setup()
{
    // az/el movement
    attachInterrupt(digitalPinToInterrupt(IIAZ), azinterrupt, CHANGE);
    attachInterrupt(digitalPinToInterrupt(IIEL), elinterrupt, CHANGE);
}

/****************************
 * Web server routines
******************************/

// replace in a simplified jinja2 way
String processor(const String &var)
{
    // debug
// #ifdef DEBUG
//     Serial.print("Processor: ");
//     Serial.println(var);
// #endif

    // azimuth
    if (var == "az")
        return String(azimuth);

    // elavation
    if (var == "el")
        return String(elevation);

    // tazimuth
    if (var == "taz")
        return String(tazimuth);

    // telavation
    if (var == "tel")
        return String(televation);

    // status
    if (var == "st")
        return String(state);

    // rssi
    if (var == "rs")
        return String(WiFi.RSSI());

    // default returns
    return String("");
}

// 404 handler >  redir to /
void notFound(AsyncWebServerRequest *request)
{
    request->send(404);
}

// ico
void serveIcon(AsyncWebServerRequest *request)
{
    request->send(LittleFS, "/favicon.ico", "image/x-icon");
#ifdef DEBUG
    Serial.println("/ico");
#endif
}

// index or root of the webserver
void serveIndex(AsyncWebServerRequest *request)
{
    request->send(LittleFS, "/index.html", String(), false, processor);
#ifdef DEBUG
    Serial.println("/ ");
#endif
}

// css
void serveStyle(AsyncWebServerRequest *request)
{
    request->send(LittleFS, "/style.css", "text/css");
#ifdef DEBUG
    Serial.println("/css");
#endif
}

// back
void serveBackMap(AsyncWebServerRequest *request)
{
    request->send(LittleFS, "/back.png", "image/x-png");
#ifdef DEBUG
    Serial.println("/back");
#endif
}

// target coordinates
void servePosition(AsyncWebServerRequest *request)
{
    request->send_P(200, "text/txt", pstatus, processor);
#ifdef DEBUG
    Serial.println("/p");
#endif
}

// server bookmarks
void serveBookmarks(AsyncWebServerRequest *request)
{
    // vars results
    String result = "{\"b\": [";

    // list all files on the b directory
    Dir dir = LittleFS.openDir("/bmk");
    String line = "";
    byte counter = 0;
    while (dir.next())
    {
        // dir.fileName();
        if (dir.isFile() && dir.fileSize() > 0)
        {
            File f = dir.openFile("r");
            String line = f.readStringUntil('#');
            f.close();

            // split the 
            String n = getValue(line, 37, 0); // 37 is '%'
            String a = getValue(line, 37, 1);
            String e = getValue(line, 37, 2);

            // more than one line
            if (counter > 0)
                result += ",";

            // actual data
            result += "{\"f\":\"" + String(dir.fileName()) + "\", \"n\":\"" + n + "\", \"a\":\"" + a + "\", \"e\":\"" + e + "\"}";

            // counter
            counter += 1;
        }
    }

    result += "]}"; 

    request->send_P(200, "text/txt", result.c_str());

#ifdef DEBUG
    Serial.println("/b");
#endif
}

// process web commands
void webCommands()
{
    switch (wcmd)
    {
    case STOP:
        wcmd = NONE;
        full_stop();
        break;

    case CAL:
        wcmd = NONE;
        calibration();
        break;

    case PARK:
        wcmd = NONE;
        state = PARKING;
        break;

    case UP:
        wcmd = NONE;
        state = MANUAL;
        move_up();
        break;

    case DOWN:
        wcmd = NONE;
        state = MANUAL;
        move_down();
        break;

    case LEFT:
        wcmd = NONE;
        state = MANUAL;
        move_left();
        break;

    case RIGHT:
        wcmd = NONE;
        state = MANUAL;
        move_right();
        break;

    default:
        break;
    }
}

// calibration from the web interface
void serveCalibration(AsyncWebServerRequest *request)
{
    // send ok then start calibration
    request->send(200, "text/plain", F("ok"));

#ifdef DEBUG
    Serial.println(F("Doing STOP from the web"));
#endif

    // set web cmd
    wcmd = CAL;
}

// parking from the web interface
void servePark(AsyncWebServerRequest *request)
{
    // send ok then start parking
    request->send(200, "text/plain", "ok");

#ifdef DEBUG
    Serial.println("Doing STOP from the web");
#endif

    // set web cmd
    wcmd = PARK;
}

// full stop from the web interface
void serveStop(AsyncWebServerRequest *request)
{
    // send ok then full stop
    request->send(200, "text/plain", F("ok"));

#ifdef DEBUG
    Serial.println(F("Doing STOP from the web"));
#endif

    // set web cmd
    wcmd = STOP;
}

// moving UP from the web interface
void serveUp(AsyncWebServerRequest *request)
{
    // send ok then
    request->send(200, "text/plain", F("ok"));

#ifdef DEBUG
    Serial.println(F("Doing UP from the web"));
#endif

    // set web cmd
    wcmd = UP;
}

// moving down from the web interface
void serveDown(AsyncWebServerRequest *request)
{
    // send ok then
    request->send(200, "text/plain", F("ok"));

#ifdef DEBUG
    Serial.println(F("Doing DOWN from the web"));
#endif

    // set web cmd
    wcmd = DOWN;
}

// moving left from the web interface
void serveLeft(AsyncWebServerRequest *request)
{
    // send ok then
    request->send(200, "text/plain", F("ok"));

#ifdef DEBUG
    Serial.println(F("Doing LEFT from the web"));
#endif

    // set web cmd
    wcmd = LEFT;
}

// moving right from the web interface
void serveRight(AsyncWebServerRequest *request)
{
    // send ok then
    request->send(200, "text/plain", F("ok"));

#ifdef DEBUG
    Serial.println(F("Doing RIGHT from the web"));
#endif

    // set web cmd
    wcmd = RIGHT;
}

// move to a specific position
void setPosition(AsyncWebServerRequest *request)
{
    //vars
    String saz = "";
    String sel = "";

    // get values if passed
    if (request->hasParam("saz"))
        saz = request->getParam("saz")->value();
    if (request->hasParam("sel"))
        sel = request->getParam("sel")->value();

    // check if az/el was passed
    if (saz != "" && sel != "")
    {
#ifdef DEBUG
        Serial.println(F("Got coordinates from serial"));
#endif

        // set target position
        tazimuth = saz.toFloat();
        televation = sel.toFloat();

        // check limits
        if (tazimuth > MAXAZIMUTH)
            tazimuth = MAXAZIMUTH;
        if (tazimuth < MINAZIMUTH)
            tazimuth = MINAZIMUTH;
        if (televation > MAXELEVATION)
            televation = MAXELEVATION;
        if (televation < MINELEVATION)
            televation = MINELEVATION;

        // force move
        state = TRACKING;
        need2move();
    }

    // send ok then
    request->send(200, "text/plain", F("ok SET"));
}

// setup the web server
void webserver_setup()
{
    // Route for root / web page
    webServer.on("/", HTTP_GET, serveIndex);

    // Static content
    webServer.on("/index.html", HTTP_GET, serveIndex);
    webServer.on("/favicon.ico", HTTP_GET, serveIcon);
    webServer.on("/style.css", HTTP_GET, serveStyle);

    // Get target position (JSON)
    webServer.on("/p", HTTP_GET, servePosition);

    // get the bookmarks
    webServer.on("/b.txt", HTTP_GET, serveBookmarks);

    // Commands
    webServer.on("/cal", HTTP_GET, serveCalibration);
    webServer.on("/stop", HTTP_GET, serveStop);
    webServer.on("/park", HTTP_GET, servePark);
    webServer.on("/up", HTTP_GET, serveUp);
    webServer.on("/down", HTTP_GET, serveDown);
    webServer.on("/left", HTTP_GET, serveLeft);
    webServer.on("/right", HTTP_GET, serveRight);
    webServer.on("/set", HTTP_GET, setPosition);

    // not found
    webServer.onNotFound(notFound);

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

    // load config
    loadConf(conf);

    // timer ticks to save the position every some time
    timer.every(AUTOSAVE_INTERVAL * 1000, confSave);

    // FS
    if (!LittleFS.begin())
    {
        Serial.println("An Error has occurred while mounting LittleFS");
        return;
    }

    Serial.print("azdratio: ");
    Serial.print(azdratio);
    Serial.print(" | eldratio: ");
    Serial.println(eldratio);

    Serial.println("");
    Serial.println("Init Parking...");
    state = PARKING;
    parking();
}

// main loop
void loop()
{
    // multiwifi run
    wifiMulti.run();

    // ota handler
    ArduinoOTA.handle();

    // catch web commands
    webCommands();

    // check limits
    limits();

    // timer tick
    timer.tick();

    // know the real status (moving/stopped)
    ismoving();

    // check if the position changed
    update_position();

    // calibrating
    if (state != CALIBRATING)
    {
        // parking
        if (state == PARKING)
        {
            parking();
        }
        else
        {
            // need to move, always AFTER the update_position one
            need2move();
        }
    }

    // mDNS update
    MDNS.update();

    // delay to make the wifi handling
    delay(1);
}
