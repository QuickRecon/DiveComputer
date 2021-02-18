//
// Created by aren on 14/2/21.
//

#ifndef DIVECOMPUTER_WEBSERVER_H
#define DIVECOMPUTER_WEBSERVER_H

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "FSInterface.h"

#define AP_SSID "Aren's Dive Computer"
#define AP_PASSWORD "12345678"

extern IPAddress local_ip;
extern IPAddress gateway;
extern IPAddress subnet;

extern ESP8266WebServer server;


void StartWebServer();

void StopWebServer();

void handle_Root();

void handle_NotFound();

void handle_Settings();

void handle_Clear();

#endif //DIVECOMPUTER_WEBSERVER_H
