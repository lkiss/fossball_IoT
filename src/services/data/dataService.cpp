#include "dataService.h"

DataService::DataService(ConfigService configService)
{
    this->configurationService = configService;
}

int DataService::sendMatchData(String &matchDataJson, String &sensorId, String &sensorNumber)
{
    Configuration config = this->configurationService.getConfiguration();
    String requestUrl = config.appServer + "/" + sensorId + "/" + sensorNumber + "/reading";

    httpClient.begin(requestUrl);
    httpClient.addHeader("Content-Type", "application/json");

    int httpStatusCode = httpClient.POST(matchDataJson);

    httpClient.end();

    return httpStatusCode;
}

String DataService::getConfiguration(String &sensorId, String &sensorNumber)
{
    Configuration config = this->configurationService.getConfiguration();
    String configPayload = "";

    String requestUrl = config.appServer + "/" + sensorId + "/" + sensorNumber + "/configuration";

    httpClient.begin(requestUrl);

    int httpStatusCode = httpClient.GET();

    if (httpStatusCode == HTTP_CODE_OK)
    {
        configPayload = httpClient.getString();
    }

    httpClient.end();

    return configPayload;
}

String DataService::getsensorConfiguration(String &sensorId, String &sensorNumber)
{
    Configuration config = this->configurationService.getConfiguration();
    String sensorConfiguration = "";

    String requestUrl = config.appServer + "/" + sensorId + "/" + sensorNumber + "/configuration";

    // Serial.print("Request URL");
    // Serial.println(requestUrl);

    httpClient.begin(requestUrl);
    int httpStatusCode = httpClient.GET();
    if (httpStatusCode == HTTP_CODE_OK)
    {
        sensorConfiguration = httpClient.getString();
    }

    httpClient.end();
    // Serial.println("sensorConfiguration");
    // Serial.println(sensorConfiguration);
    return sensorConfiguration;
}