#ifndef SERVER_SERVICE
#define SERVER_SERVICE

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include "../config/configService.h"
#include "../../constants.h"

class ServerService
{
private:
  ConfigService configService;
  ESP8266WebServer *server;
  bool isConfigSet = false;

  void configureAccessPoint();
  void configureServer();
  void shutDownAccessPoint();
  void connectToWifiNetwork();
  void startListening();
  void dispose();

public:
  ServerService();
  void start();
};

#endif