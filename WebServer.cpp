//
// Created by aren on 14/2/21.
//

#include "WebServer.h"

IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

ESP8266WebServer server(80);

std::vector<std::string> diveList;

void StartWebServer() {
    WiFi.forceSleepWake(); // Enable wifi
    WiFi.softAP(AP_SSID, AP_PASSWORD);
    WiFi.softAPConfig(local_ip, gateway, subnet);
    delay(100);
    diveList = ListDives();

    server.begin();
    server.on("/", handle_Root);
    server.on("/settings", handle_Settings);
    server.on("/clear", handle_Clear);
    server.onNotFound(handle_NotFound);
    int k = 0;
    for (auto &i : diveList) {
        String path = DIVE_LOG_PATH;
        server.serveStatic(("/" + String(k) + ".csv").c_str(), LittleFS, (path + i.c_str()).c_str());
        k++;
    }
}

void StopWebServer() {
    server.close();
    server.stop();
    WiFi.forceSleepBegin();
}

void handle_NotFound() {
    server.send(404, "text/plain", "Not found");
}

void handle_Settings() {
    String html = "<!DOCTYPE html> <html>\n";
    html += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
    html += "<title>Dive Computer Manager</title>\n";
    html += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
    html += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
    html += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
    html += "</style>\n";
    html += "</head>\n";
    html += "<body>\n";
    html += "</body>\n";
    html += "</html>\n";
    server.send(200, "text/html", html);
}

void handle_Clear() {
    handle_Root();
}

void handle_Root() {
    String html = "<!DOCTYPE html> <html>\n";
    html += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
    html += "<title>Dive Computer Manager</title>\n";
    html += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
    html += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
    html += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
    html += ".dive {display: block;width:80px;background-color: #09BC8A;border: none;color: white;padding: 8px 15px;text-decoration: none;font-size: 15px;margin: 0px auto 10px;cursor: pointer;border-radius: 4px;}\n";
    html += ".settings {display:inline-block;;width:80px;background-color: #058ED9;border: none;color: white;padding: 8px 15px;text-decoration: none;font-size: 15px;margin: 0px auto 10px;cursor: pointer;border-radius: 4px;}\n";
    html += ".clear {display:inline-block;;width:80px;background-color: #F4442E;border: none;color: white;padding: 8px 15px;text-decoration: none;font-size: 15px;margin: 0px auto 10px;cursor: pointer;border-radius: 4px;}\n";
    html += "</style>\n";
    html += "</head>\n";
    html += "<body>\n";
    html += "<h1>Dive Computer Manager</h1>\n";
    html += "<a class=\"settings\" href=\"/settings\">Settings</a>\n";
    html += "<a class=\"clear\" href=\"/clearDives\">Clear Dives</a>\n";
    html += "<h2>Dives:</h2>";
    int k = 0;
    for (auto &i : diveList) {
        html += "<a class=\"dive\" href=";
        html += ("\"/" + String(k) + ".csv\">").c_str();
        html += i.c_str();
        html += "</a>\n";
        k++;
    }
    html += "</body>\n";
    html += "</html>\n";
    server.send(200, "text/html", html);
}