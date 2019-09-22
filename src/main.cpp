#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#define WIFI_SSID "PNSC_2_4"
#define WIFI_PASSWORD "5113111_Cons!"

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

void sendMatchHistory(String buttonId)
{
    client.begin("http://us-central1-hubsson-foosball-eur3.cloudfunctions.net/matchHistory");
    client.addHeader("Content-Type", "application/json");

    String payload = "{\"buttonId\" :";
    payload += "\"" + buttonId + "\"}";

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
        sendMatchHistory(redStriker);
        delay(buttonDelayInMilliseconds);
    }
    if (redDefenderRead == LOW)
    {
        Serial.println("Red 2 button is pressed");
        sendMatchHistory(redDefender);
        delay(buttonDelayInMilliseconds);
    }
    if (blueStrikerRead == LOW)
    {
        Serial.println("Blue 1 button is pressed");
        sendMatchHistory(blueStriker);
        delay(buttonDelayInMilliseconds);
    }
    if (blueDefenderRead == LOW)
    {
        Serial.println("Blue 2 button is pressed");
        sendMatchHistory(blueDefender);
        delay(buttonDelayInMilliseconds);
    }
    if (resetRead == LOW)
    {
        Serial.println("Reset button is pressed");
        sendMatchHistory(reset);
        delay(buttonDelayInMilliseconds);
    }
}