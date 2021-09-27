#include <network.h>

void Network::setupWifi(char* ssid, char* pwd)
{
  delay(10);
  Serial.println("connecting WIFI");
  WiFi.begin(ssid, pwd);
}

void Network::setUpHttpClient(char* host)
{
//   req = (String)host;
//   Serial.println(req);
//   if (http_client.begin(req))
//   {
//     Serial.println("HTTPclient setUp done!");
//   }
}