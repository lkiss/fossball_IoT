#ifndef CONFIG_SERVICE
#define CONFIG_SERVICE

#include <Arduino.h>
#include <EEPROM.h>
#include "config.h"
#include "../json/jsonService.h"

class ConfigService
{
private:
  JsonService jsonService;
  int ConfigurationAddress = 0;

  int saveConfig(String value, bool isReset);
  String getConfig();

public:
  ConfigService();
  ConfigService(JsonService jsonService);

  Configuration getConfiguration();
  void setConfiguration(String config);
  bool isCloudConfigured();
};

#endif