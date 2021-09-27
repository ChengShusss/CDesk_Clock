#include <Arduino.h>
#include <Bounce2.h>
#include <WiFi.h>
#include <HTTPClient.h>

#include "clock.h"
#include "main.h"
#include "display.h"
#include "utils.h"
#include "network.h"

#define PIN_SW 32
#define DEBOUNCE_TIME 10 //延时用来过滤不正常的信号，

Clock clk = Clock();
Display display = Display();
Bounce sw = Bounce();

Ds1302::DateTime before;
Ds1302::DateTime now;
int8_t state = 0;
int16_t var_x;
int16_t var_y;
char *ssid = "Shadow.CHENG";                      //wifi名
char *password = "cWsMwifi";              //wifi密码
char *host = "http://www.beijing-time.org/t/time.asp"; //url
// WiFiClient wifi_Client;
HTTPClient http_client;
Network* network = new Network();
String req;
String rsp;

unsigned char rocker_state = 0;
const static char* Menuitems[] ={
    "Normal",
    "Setting",
    "About",
    "Time Set",
};
const static int menuJump[4][6] = {
    {0, 0, 0, 0, 1},
    {2, 2, 1, 1, 3},
    {1, 1, 2, 2, 2},
    {3, 3, 1, 2, 0},
};


const static char* WeekDays[] =
{
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday",
    "Sunday"
};

// void setupWifi()
// {
//   delay(10);
//   Serial.println("connecting WIFI");
//   WiFi.begin(ssid, password);
//   while (!WiFi.isConnected())
//   {
//     Serial.print(".");
//     delay(500);
//   }
//   Serial.println("OK");
//   Serial.println("Wifi connected");
// }

// void setUpHttpClient()
// {
//   req = (String)host;
//   Serial.println(req);
//   if (http_client.begin(req))
//   {
//     Serial.println("HTTPclient setUp done!");
//   }
// }


void setup()
{
    Serial.begin(115200);
    display.clean();

    WiFi.begin();
    
    for(unsigned char i = 0; i <= 100; i += 5){
        if(network->autoConfig()){
            i = 100;
            display.showInitBar(i);
            break;
        }
        display.showInitBar(i);
        delay(500);
    }
    
    delay(1000);
    
    if (!network->isConnected()){
        network->wifiConfig();
    }

    sw.attach(PIN_SW, INPUT_PULLUP);
    

    //用于删除已存WiFi
    if (digitalRead(PIN_SW) == LOW) {
        delay(1000);
        esp_wifi_restore();
        delay(10);
        ESP.restart();  //复位esp32
    }

    display.clean();
    
    sw.interval(DEBOUNCE_TIME);

    // test if clock is halted and set a date-time (see example 2) to start it
    if (clk.isHalted())
    {
        Serial.println("RTC is halted. Setting time...");
        clk.setTime(21, 9, 25, 21, 16, 30, 7);
    }

    clk.getTime(&before);
    clk.getTime(&now);

    printTime();
    display.drawFrame();

    // delay(3000);
    // network->setupWifi(ssid, password);
    // network->setUpHttpClient(host);
}


void loop()
{
    network->handleClient();
    
    // get the current time
    sw.update();
    rocker_state = scanRocker();
    if (rocker_state != 255){
        state = menuJump[state][rocker_state];
        printMenu();
    }
    if(sw.fell()){
        Serial.print("Pushed button.");
        state = menuJump[state][4];
        printMenu();
    }

    clk.getTime(&now);
    if (before.second != now.second){
        printTime();
        copyDateTime(&now, &before);
    }

    // int http_code = http_client.GET();
    // Serial.println(http_code);
    // if (http_code > 0)
    // {
    //     Serial.printf("HTTP get code: %d\n", http_code);
    //     if (http_code == HTTP_CODE_OK)
    //     {
    //     rsp = http_client.getString();
    //     Serial.println(rsp);
    //     }
    //     else
    //     {
    //     Serial.printf("fail to get cityWeather,code:%d\n", http_code);
    //     }
    // }
    // delay(5000);
    
}

unsigned char scanRocker(void)
{
  static unsigned char keyUp = 1;

  var_x = analogRead(35) >> 2;
  var_y = analogRead(34) >> 2;

  if (keyUp && ((var_x <= 10) || (var_x >= 1010) || (var_y <= 10) || (var_y >= 1010)))
  {
    delay(10);
    keyUp = 0;
    if (var_x <= 10)return 0;
    else if (var_x >= 1010)return 1;
    else if (var_y >= 1010)return 2;
    else if (var_y <= 10)return 3;
  } else if ((var_x > 10) && (var_x < 1010) && (var_y > 10) && (var_y < 1010))keyUp = 1;
  return 255;
}


void printMenu(){
    display.tft.fillRect(2, 72, 90, 16, ST77XX_BLUE);
    display.tft.setTextColor(ST77XX_ORANGE);
    display.tft.setCursor(4, 72);
    display.tft.setTextSize(2);
    display.tft.print(Menuitems[state]);
}



void printTime(void){
    display.drawTime(&before, &now);
    #ifdef DEBUG_CS
        Serial.print("20");
        Serial.print(now.year);    // 00-99
        Serial.print('-');
        if (now.month < 10) Serial.print('0');
        Serial.print(now.month);   // 01-12
        Serial.print('-');
        if (now.day < 10) Serial.print('0');
        Serial.print(now.day);     // 01-31
        Serial.print(' ');
        Serial.print(WeekDays[now.dow - 1]); // 1-7
        Serial.print(' ');
        if (now.hour < 10) Serial.print('0');
        Serial.print(now.hour);    // 00-23
        Serial.print(':');
        if (now.minute < 10) Serial.print('0');
        Serial.print(now.minute);  // 00-59
        Serial.print(':');
        if (now.second < 10) Serial.print('0');
        Serial.print(now.second);  // 00-59
        Serial.println();

        Serial.print("rocker-x: ");
        Serial.print(var_x);
        Serial.print("  rocker-y: ");
        Serial.println(var_y);

        if(!network->isConnected()){
            Serial.println("Try to connect wifi...");
        }
    #endif
}