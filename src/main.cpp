#include <Arduino.h>
#include "./services/services.h"

uint red1Pin = 4;
uint red2Pin = 5;
uint blue1Pin = 14;
uint blue2Pin = 12;
uint resetPin = 13;
uint buttonDelayInMilliseconds = 200;

void setup()
{
    Serial.begin(9600);
    pinMode(red1Pin, INPUT_PULLUP);
    pinMode(red2Pin, INPUT_PULLUP);
    pinMode(blue1Pin, INPUT_PULLUP);
    pinMode(blue2Pin, INPUT_PULLUP);
    pinMode(resetPin, INPUT_PULLUP);
}

void loop()
{
    int red1 = digitalRead(red1Pin);
    int red2 = digitalRead(red2Pin);
    int blue1 = digitalRead(blue1Pin);
    int blue2 = digitalRead(blue2Pin);
    int reset = digitalRead(resetPin);

    if (red1 == LOW)
    {
        Serial.println("Red 1 button is pressed");
        delay(buttonDelayInMilliseconds);
    }
    if (red2 == LOW)
    {
        Serial.println("Red 2 button is pressed");
        delay(buttonDelayInMilliseconds);
    }
    if (blue1 == LOW)
    {
        Serial.println("Blue 1 button is pressed");
        delay(buttonDelayInMilliseconds);
    }
    if (blue2 == LOW)
    {
        Serial.println("Blue 2 button is pressed");
        delay(buttonDelayInMilliseconds);
    }
    if (reset == LOW)
    {
        Serial.println("Reset button is pressed");
        delay(buttonDelayInMilliseconds);
    }
}