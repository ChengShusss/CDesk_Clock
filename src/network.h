#ifndef _NETWORK_CDC
#define _NETWORK_CDC

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <esp_wifi.h>



class Network{
private:
    WebServer server;
    HTTPClient http_client;
    const char* AP_SSID; //热点名称
    String wifi_ssid;
    String wifi_pass;
    String scanNetworksID;//用于储存扫描到的WiFi
    String req;
    String rsp;
    // char* ssid;
    // char* pwd;
public:
    Network();
    void setupWifi(char* ssid, char* pwd);

    void wifiConfig(void);
    bool autoConfig(void);
    void handleClient(void);
    
    void setUpHttpClient(char* host);

    bool isConnected(void);
};




#define ROOT_HTML  "<!DOCTYPE html><html><head><title>WIFI Config by lwang</title><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head><style type=\"text/css\">.input{display: block; margin-top: 10px;}.input span{width: 100px; float: left; float: left; height: 36px; line-height: 36px;}.input input{height: 30px;width: 200px;}.btn{width: 120px; height: 35px; background-color: #000000; border:0px; color:#ffffff; margin-top:15px; margin-left:100px;}</style><body><form method=\"GET\" action=\"connect\"><label class=\"input\"><span>WiFi SSID</span><input type=\"text\" name=\"ssid\"></label><label class=\"input\"><span>WiFi PASS</span><input type=\"text\"  name=\"pass\"></label><input class=\"btn\" type=\"submit\" name=\"submit\" value=\"Submie\"> <p><span> Nearby wifi:</P></form>"


#endif