#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include "../config/configService.h"
#include "../../constants.h"

class WifiService
{
  private:
    ConfigService configService;
    bool isWifiConfigured();

  public:
    WifiService(ConfigService configService);
    void connectIfConfigured();
};