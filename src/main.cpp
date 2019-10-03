#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#define WIFI_SSID "Guestsson"
#define WIFI_PASSWORD "X6JSO89Z"

HTTPClient client;

uint red1Pin = 4;
uint red2Pin = 5;
uint blue1Pin = 14;
uint blue2Pin = 12;
uint resetPin = 13;
uint buttonDelayInMilliseconds = 200;

String blueStriker = "BLUE_STRIKER";
String blueDefender = "BLUE_DEFENDER";
String redStriker = "RED_STRIKER";
String redDefender = "RED_DEFENDER";
String reset = "RESET";

String red = "red";
String blue = "blue";
String defence = "blue";
String striker = "blue";

void setup()
{
    Serial.begin(9600);

    pinMode(red1Pin, INPUT_PULLUP);
    pinMode(red2Pin, INPUT_PULLUP);
    pinMode(blue1Pin, INPUT_PULLUP);
    pinMode(blue2Pin, INPUT_PULLUP);
    pinMode(resetPin, INPUT_PULLUP);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("connecting");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }
    Serial.println();
    Serial.print("connected: ");
    Serial.println(WiFi.localIP());
}

String getActionType(int pin)
{
    delay(2000);
    return digitalRead(pin) == LOW ? "undo" : "goal";
}

void preventRequestFlood(int pin)
{
    while (digitalRead(pin) == LOW)
    {
        yield();
    }
}

void sendMatchHistory(String team, String position, String actionType)
{
    client.begin("http://us-central1-hubsson-foosball-eur3.cloudfunctions.net/matchHistory");
    client.addHeader("Content-Type", "application/json");

    String payload = "{\"team\" :";
    payload += "\"" + team + "\",";
    payload += "\"position\" :";
    payload += "\"" + position + "\",";
    payload += "\"action\" :";
    payload += "\"" + actionType + "\"}";

    Serial.println("Sending payload");
    Serial.println(payload);

    int errorCode = client.PUT(payload);

    if (errorCode != HTTP_CODE_OK)
    {
        Serial.println(client.errorToString(errorCode));
    }

    client.end();
}

void loop()
{
    int redStrikerRead = digitalRead(red1Pin);
    int redDefenderRead = digitalRead(red2Pin);
    int blueStrikerRead = digitalRead(blue1Pin);
    int blueDefenderRead = digitalRead(blue2Pin);
    int resetRead = digitalRead(resetPin);

    if (redStrikerRead == LOW)
    {
        Serial.println("Red 1 button is pressed");
        sendMatchHistory(red, striker, getActionType(red1Pin));
        preventRequestFlood(red1Pin);
        delay(buttonDelayInMilliseconds);
    }
    if (redDefenderRead == LOW)
    {
        Serial.println("Red 2 button is pressed");
        sendMatchHistory(red, defence, getActionType(red2Pin));
        preventRequestFlood(red2Pin);
        delay(buttonDelayInMilliseconds);
    }
    if (blueStrikerRead == LOW)
    {
        Serial.println("Blue 1 button is pressed");
        sendMatchHistory(blue, striker, getActionType(blue1Pin));
        preventRequestFlood(blue1Pin);
        delay(buttonDelayInMilliseconds);
    }
    if (blueDefenderRead == LOW)
    {
        Serial.println("Blue 2 button is pressed");
        sendMatchHistory(blue, defence, getActionType(blue2Pin));
        preventRequestFlood(blue2Pin);
        delay(buttonDelayInMilliseconds);
    }
    if (resetRead == LOW)
    {
        Serial.println("Reset button is pressed");
        sendMatchHistory(reset, "", "reset");
        preventRequestFlood(resetPin);
        delay(buttonDelayInMilliseconds);
    }
}