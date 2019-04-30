#include "./serverService.h"

ServerService::ServerService()
{
    this->server = new ESP8266WebServer(LOCAL_IP, PORT);
}

void ServerService::configureAccessPoint()
{
    WiFi.softAPConfig(LOCAL_IP, GATEWAY, SUBNET);
    WiFi.softAP("PlantManager_Device");
}

void ServerService::configureServer()
{
    ConfigService configService = this->configService;

    this->server->on("/", HTTP_GET, [this]() {
        this->server->send(200);
        delay(300);
    });

    server->on("/", HTTP_PUT, [this]() {
        if (server->hasArg("plain") == false)
        {
            server->send(400);
        }
        else
        {
            this->configService.setConfiguration(server->arg("plain"));
            server->send(200, "application/json", server->arg("plain"));
            delay(300);
            this->isConfigSet = true;
        }
    });
}

void ServerService::startListening()
{
    this->server->begin();

    while (!this->isConfigSet)
    {
        this->server->handleClient();
    }

    this->dispose();
}

void ServerService::dispose()
{
    this->server->stop();
    this->server->close();
    this->server->~ESP8266WebServer();
}

void ServerService::shutDownAccessPoint()
{
    ConfigService configService = this->configService;

    Configuration config = configService.getConfiguration();
    WiFi.mode(WIFI_AP_STA);
    WiFi.enableAP(false);
}

void ServerService::start()
{
    this->configureAccessPoint();
    this->configureServer();
    this->startListening();
    this->shutDownAccessPoint();
}