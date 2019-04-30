#ifndef DATA_SERVICE
#define DATA_SERVICE

#include "../config/configService.h"

#include <Arduino.h>
#include <ESP8266HTTPClient.h>

class DataService
{
private:
  HTTPClient httpClient;
  ConfigService configurationService;

public:
  DataService(ConfigService configService);
  int sendMatchData(String &matchDataJson, String &sensorId, String &sensorNumber);
  String getConfiguration(String &sensorId, String &sensorNumber);
  String getsensorConfiguration(String &sensorId, String &sensorNumber);
};

#endif