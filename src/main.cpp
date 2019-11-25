#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
// #define WIFI_SSID "Guestsson"
// #define WIFI_PASSWORD "X6JSO89Z"

#define WIFI_SSID "PNSC_2_4"
#define WIFI_PASSWORD "5113111_Cons!"

HTTPClient client;

uint red1Pin = 4;
uint red2Pin = 5;
uint blue1Pin = 14;
uint blue2Pin = 12;
uint resetPin = 13;
uint buttonDelayInMilliseconds = 1000;

String blueStriker = "BLUE_STRIKER";
String blueDefender = "BLUE_DEFENDER";
String redStriker = "RED_STRIKER";
String redDefender = "RED_DEFENDER";
String reset = "RESET";

String red = "red";
String blue = "blue";
String defender = "defender";
String striker = "striker";

struct Player
{
    String position = "";
    String team = "";
};
struct Action
{
    String actionType = "";
    String team01 = "";
    String position01 = "";
    String team02 = "";
    String position02 = "";
};

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

Player getPlayer(int currentPin)
{
    Player player;

    if (digitalRead(red1Pin) == LOW && red1Pin != currentPin)
    {
        player.position = striker;
        player.team = red;
    }
    else if (digitalRead(red2Pin) == LOW && red2Pin != currentPin)
    {
        player.position = defender;
        player.team = red;
    }
    else if (digitalRead(blue1Pin) == LOW && blue1Pin != currentPin)
    {
        player.position = striker;
        player.team = blue;
    }
    else if (digitalRead(blue2Pin) == LOW && blue2Pin != currentPin)
    {
        player.position = defender;
        player.team = blue;
    }

    return player;
}

Action getAction(int pin, Player player)
{
    delay(10);
    int pulseTime = pulseIn(pin, HIGH, 2000000);

    Serial.print("PulseTime: ");
    Serial.println(pulseTime);

    Action action;
    action.team01 = player.team;
    action.position01 = player.position;

    if (pulseTime != 0 && pulseTime >= 100 && pulseTime <= 2000000)
    {
        action.actionType = "ownGoal";
        return action;
    }

    delay(2000);

    if (digitalRead(pin) == LOW)
    {
        Player swapTeam = getPlayer(pin);
        if (swapTeam.team != "")
        {
            action.team02 = swapTeam.team;
            action.position02 = swapTeam.position;
            action.actionType = "swap";
            return action;
        }

        action.actionType = "undo";
        return action;
    }

    action.actionType = "goal";
    return action;
}

void preventRequestFlood(int pin)
{
    while (digitalRead(pin) == LOW)
    {
        yield();
    }
}

void sendMatchHistory(Action action)
{
    client.begin("http://us-central1-hubsson-foosball-eur3.cloudfunctions.net/matchHistory");
    client.addHeader("Content-Type", "application/json");

    String payload = "{\"team01\" :";
    payload += "\"" + action.team01 + "\",";
    payload += "\"position01\" :";
    payload += "\"" + action.position01 + "\",";
    payload += "\"action\" :";
    payload += "\"" + action.actionType + "\"";

    if (action.team02 != "")
    {
        payload += ",\"team02\" :";
        payload += "\"" + action.team02 + "\",";
        payload += "\"position02\" :";
        payload += "\"" + action.position02 + "\"";
    }

    payload += "}";

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

        Player redStrikerPlayer;
        redStrikerPlayer.team = red;
        redStrikerPlayer.position = striker;

        Action redStrikerAction = getAction(red1Pin, redStrikerPlayer);

        sendMatchHistory(redStrikerAction);
        preventRequestFlood(red1Pin);
        delay(buttonDelayInMilliseconds);
    }
    if (redDefenderRead == LOW)
    {
        Serial.println("Red 2 button is pressed");

        Player redDefenderPlayer;
        redDefenderPlayer.team = red;
        redDefenderPlayer.position = defender;

        Action redDefenderAction = getAction(red2Pin, redDefenderPlayer);

        sendMatchHistory(redDefenderAction);

        preventRequestFlood(red2Pin);
        delay(buttonDelayInMilliseconds);
    }
    if (blueStrikerRead == LOW)
    {
        Serial.println("Blue 1 button is pressed");
        Player blueStrikerPlayer;
        blueStrikerPlayer.team = blue;
        blueStrikerPlayer.position = striker;

        Action blueStrikerAction = getAction(blue1Pin, blueStrikerPlayer);

        sendMatchHistory(blueStrikerAction);

        preventRequestFlood(blue1Pin);
        delay(buttonDelayInMilliseconds);
    }
    if (blueDefenderRead == LOW)
    {
        Serial.println("Blue 2 button is pressed");
        Player blueDefenderPlayer;
        blueDefenderPlayer.team = blue;
        blueDefenderPlayer.position = defender;

        Action blueDefenderAction = getAction(blue1Pin, blueDefenderPlayer);

        sendMatchHistory(blueDefenderAction);
        preventRequestFlood(blue2Pin);
        delay(buttonDelayInMilliseconds);
    }
    if (resetRead == LOW)
    {
        Serial.println("Reset button is pressed");
        Action resetAction;
        resetAction.actionType = reset;

        sendMatchHistory(resetAction);
        preventRequestFlood(resetPin);
        delay(buttonDelayInMilliseconds);
    }
}