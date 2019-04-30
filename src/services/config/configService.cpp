#include "configService.h"

ConfigService::ConfigService()
{
}

ConfigService::ConfigService(JsonService jsonService)
{
  this->jsonService = jsonService;
}

Configuration ConfigService::getConfiguration()
{
  String jsonConfig = this->getConfig();
  // Serial.println("Get Configuration: ");
  // Serial.println(jsonConfig);
  Configuration config = this->jsonService.convertJsonToConfig(jsonConfig);

  // Serial.println(config.sensorConfiguration);
  return config;
}

void ConfigService::setConfiguration(String config)
{
  // Serial.println("SetConfiguration");
  this->saveConfig(config, false);

  // String jsonConfig = this->getConfig();
  // Serial.println("Before Jsonconvert");
  // Configuration configuration = this->jsonService.convertJsonToConfig(jsonConfig);
  // Serial.println("After Jsonconvert");
}

bool ConfigService::isCloudConfigured()
{
  Configuration configuration = this->getConfiguration();

  return configuration.appServer != "" && configuration.ssid != "" && configuration.password != "";
}

int ConfigService::saveConfig(String value, bool isReset)
{
  EEPROM.begin(512);
  // Serial.println("SaveConfig");
  int stringLength = isReset ? 512 : value.length();

  if (stringLength == 0 || value == NULL)
  {
    return 0;
  }

  if (isReset)
  {
    for (int i = 0; i <= value.length(); i++)
    {
      if (i == value.length())
      {
        EEPROM.write(i, '\0');
        break;
      }
      EEPROM.write(i, 0);
    }
  }
  else
  {
    for (int i = 0; i <= value.length(); i++)
    {
      if (i == value.length())
      {
        EEPROM.write(i, '\0');
        break;
      }
      EEPROM.write(i, value[i]);
    }
  }

  EEPROM.commit();
  EEPROM.end();

  return stringLength;
}

String ConfigService::getConfig()
{
  EEPROM.begin(512);
  // Serial.println("ReadConfig");
  char buffer[EEPROM.length()];
  for (int i = 0; i < EEPROM.length(); i++)
  {
    buffer[i] = EEPROM.read(i);
    if (EEPROM.read(i) == '\0')
    {
      break;
    }
  }

  EEPROM.end();
  return buffer;
}