// code exclusive for ESP8266
#if !(defined(ESP8266))
#error This code is intended to run on ESP8266 platform! Please check your Tools->Board setting.
#endif

// libraries
#include <ESPAsync_WiFiManager.h> //https://github.com/khoih-prog/ESPAsync_WiFiManager
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <DNSServer.h>
#include <vector>

#include "config.h"

static DNSServer DNS;
AsyncWebServer webServer(80);
static std::vector<AsyncClient *> clients; // a list to hold all clients

/****************************************************/
float azimuth = 0.0;
float elevation = 0.0;

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
    azimuth = a;
    elevation = e;

    Serial.print("P ");
    Serial.print(azimuth);
    Serial.print(" ");
    Serial.println(elevation);
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
    Serial.println("S");
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

    //Serial.println(data);
    Serial.println(result);
    return result;
}

/**************************
 * TCP socket part
 ***************************/
static void handleError(void *arg, AsyncClient *client, int8_t error)
{
    Serial.printf("\n connection error %s from client %s \n", client->errorToString(error), client->remoteIP().toString().c_str());
}

static void handleData(void *arg, AsyncClient *client, void *data, size_t len)
{
    char buff[32];
    memset(buff, '\0', 32);
    sprintf(buff, "%s", data);

    // send the data to the handle function
    String answer = msg_handle(buff);

    // reply to client
    if (client->space() > 32 && client->canSend())
    {
        char reply[32];
        sprintf(reply, "%s\n", answer.c_str());
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
    Serial.printf("\n new client has been connected to server, ip: %s", client->remoteIP().toString().c_str());

    // add to list
    clients.push_back(client);

    // register events
    client->onData(&handleData, NULL);
    client->onError(&handleError, NULL);
    client->onDisconnect(&handleDisconnect, NULL);
    client->onTimeout(&handleTimeOut, NULL);
}

void handleInterrupt()
{
    Serial.println("Interrupt Detected");
}

void setup()
{
    Serial.begin(115200);
    delay(200);

    Serial.print("\nStarting Async_AutoConnect_ESP8266_minimal on " + String(ARDUINO_BOARD));

    Serial.println(ESP_ASYNC_WIFIMANAGER_VERSION);
    WiFi.persistent(true);
    ESPAsync_WiFiManager ESPAsync_wifiManager(&webServer, &DNS, "AWRC");
    //ESPAsync_wifiManager.resetSettings();   //reset saved settings
    ESPAsync_wifiManager.autoConnect("AWRC");

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.print(F("Connected. Local IP: "));
        Serial.println(WiFi.localIP());
    }
    else
    {
        Serial.println(ESPAsync_wifiManager.getStatus(WiFi.status()));
    }

    AsyncServer *server = new AsyncServer(TCP_PORT); // start listening on tcp port 7050
    server->onClient(&handleNewClient, server);
    server->begin();

    // interrupt declaration
    // pinMode(interruptPin, INPUT_PULLUP);
    // attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, CHANGE);
}

void loop()
{
    DNS.processNextRequest();
}
