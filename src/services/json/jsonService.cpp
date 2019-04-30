#include "./jsonService.h"

Configuration JsonService::convertJsonToConfig(String &configJson)
{
    const size_t bufferSize = 2 * JSON_OBJECT_SIZE(4) + 210;
    Configuration configuration = Configuration();
    String jsonMessage;
    DynamicJsonDocument buffer(bufferSize);

    // Serial.println(configJson);
    // Serial.println("Before jsonParse");
    auto error = deserializeJson(buffer, configJson);
    if (!error)
    {
        // Serial.println("Assign values from json");
        // Serial.println(config["sensorConfiguration"].as<char *>());

        configuration.appServer = buffer["appServer"].as<String>();
        configuration.ssid = buffer["ssid"].as<String>();
        configuration.password = buffer["password"].as<String>();

        buffer.clear();
    }

    // Serial.println("After jsonParse");
    // Serial.println("sensorConfiguration from config: ");
    // Serial.println(configuration.sensorConfiguration);
    // Serial.println(config["sensorConfiguration"].as<String>());

    return configuration;
}