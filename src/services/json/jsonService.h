#ifndef JSON_SERVICE
#define JSON_SERVICE

#include <ArduinoJson.h>
#include <Arduino.h>
#include "../config/config.h"

class JsonService
{
public:
  Configuration convertJsonToConfig(String &configJson);
};

#endif