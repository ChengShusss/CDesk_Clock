#ifndef _NETWORK_CDC
#define _NETWORK_CDC

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>



class Network{
private:
    // char* ssid;
    // char* pwd;
public:
    

    void setupWifi(char* ssid, char* pwd);
    void setUpHttpClient(char* host);
};


#endif