#include "./wifiService.h"

WifiService::WifiService(ConfigService configService)
{
  this->configService = configService;
}

void WifiService::connectIfConfigured()
{
  ConfigService configService = this->configService;

  if (configService.isCloudConfigured())
  {
    Configuration config = configService.getConfiguration();
    WiFi.begin(config.ssid.c_str(), config.password.c_str());

    while (WiFi.waitForConnectResult() != WL_CONNECTED)
    {
      WiFi.begin(config.ssid.c_str(), config.password.c_str());
    }
  }
}