#include <Arduino.h>
#include <Bounce2.h>
#include <WiFi.h>
#include <HTTPClient.h>

#include "clock.h"
#include "main.h"
#include "display.h"
#include "utils.h"
#include "network.h"

#define DEBOUNCE_TIME 10 //延时用来过滤不正常的信号，

Clock clk = Clock();
Display display = Display();
Bounce sw = Bounce();

Ds1302::DateTime before;
Ds1302::DateTime now;
int8_t state = 0;
int16_t var_x;
int16_t var_y;
bool isConnected = false;
char host[] = "http://www.beijing-time.org/t/time.asp"; //url
WiFiClient wifi_Client;
HTTPClient http_client;
Network* network = new Network();
String req;
String rsp;

unsigned char rocker_state = 0;
const static char* Menuitems[] ={
    "Normal",
    "Time Set",
    "Wifi Set",
    "About",
    "Auto Sync",
    "Manual Set",
    "Connect",
    "Reset"
};

KEY_TABLE menuTable[] = {
    {0, 0, 0, 1, 1, (*printMenu)}, // 0:normal
    {3, 2, 0, 4, 4, (*printMenu)}, // 1:time set
    {1, 3, 0, 6, 6, (*printMenu)}, // 2:wifi set
    {2, 1, 0, 3, 3, (*testAbout)}, // 3:about
    {5, 5, 1, 4, 4, (*testSync)}, // 4:auto sync
    {4, 4, 1, 5, 5, (*printMenu)}, // 5:manual set
    {7, 7, 2, 6, 6, (*webWifiConfig)}, // 6:connect
    {6, 6, 2, 7, 7, (*printMenu)}  // 7:reset
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

const static char* Notes[] = {
  "今天不上班",
  "下班啦",
};
uint8_t noteIndex = 0;

const static char testChinese[] = "测试中文";
unsigned indexChinese = 0;

void setup()
{
  
    SPIFFS.begin();
    Serial.begin(115200);
    display.clean();

    sw.attach(PIN_SW, INPUT_PULLUP);
    sw.interval(DEBOUNCE_TIME);

    if (digitalRead(PIN_SW) == LOW) {
      delay(1000);
      esp_wifi_restore();
      delay(10);
      ESP.restart();  //复位esp32
    }

    WiFi.begin();
    for(unsigned char i = 0; i <= 100; i += 5){
        if(network->autoConfig()){
          i = 100;
          display.showInitBar(i);
          isConnected = true;
          break;
        }
        display.showInitBar(i);
        delay(300);
    }
    delay(500);
    display.clean();
    

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
    printMenu();
    display.openFontFile();

}


void loop()
{
    network->handleHttpRequest();
    // get the current time
    sw.update();
    rocker_state = scanRocker();
    if (rocker_state != 255){
        // state = menuJump[state][rocker_state];
        switch (rocker_state)
        {
          case 0: state = menuTable[state].up; break;
          case 1: state = menuTable[state].down; break;
          case 2: state = menuTable[state].left; break;
          case 3: state = menuTable[state].right; break;
        }
        printMenu();
    }
    if(sw.fell()){
        state = menuTable[state].sw;
        (*menuTable[state].operation)();
        // printMenu();
    }

    clk.getTime(&now);
    if (before.second != now.second){
      if(before.minute != now.minute){
        printTime();
      }
      // printInfo();
      if (isConnected){
        display.drawWifiStatus("Connected");
      }
      else{
        display.drawWifiStatus("Offline");
      }
      copyDateTime(&now, &before);

      display.drawUtf8Char(0x6d4b, 6, 88, ST7735_ORANGE);

      // display.drawUtf8String(Notes[noteIndex], 6, 108, BACKGROUND);
      noteIndex = (noteIndex + 1) % 2;
      display.drawUtf8String(Notes[noteIndex], 6, 108, ST7735_RED);
      Serial.println("测试输出");

      // test print
    }
    Serial.print("-");

}

unsigned char scanRocker(void)
{
  static unsigned char keyUp = 1;

  var_x = analogRead(35) >> 5;
  var_y = analogRead(34) >> 5;

  if (keyUp && ((var_x <= 10) || (var_x >= 120) || (var_y <= 10) || (var_y >= 120)))
  {
    delay(10);
    keyUp = 0;
    if (var_x <= 10)return 0;
    else if (var_x >= 120)return 1;
    else if (var_y >= 120)return 2;
    else if (var_y <= 10)return 3;
  } else if ((var_x > 10) && (var_x < 120) && (var_y > 10) && (var_y < 120))keyUp = 1;
  return 255;
}

void printInfo(void){
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


void printMenu(){
    display.tft.fillRect(2, 72, 90, 16, ST77XX_BLUE);
    display.tft.setTextColor(ST77XX_ORANGE);
    display.tft.setCursor(4, 72);
    display.tft.setTextSize(1);
    display.tft.print(Menuitems[state]);
}



void printTime(void){
    display.drawTime(&before, &now);
}

void testAbout(void){
  Serial.println("Here is about.");
}

void testSync(void){
  HTTPClient http; // 声明HTTPClient对象
  uint8_t dt[] = {0, 0, 0, 0, 0, 0, 0};
  uint8_t i = 0;
  uint8_t p = 0;

  http.begin(host); // 准备启用连接
  Serial.println("Start to get info.");

  int httpCode = http.GET(); // 发起GET请求

  if (httpCode > 0) // 如果状态码大于0说明请求过程无异常
  {
    if (httpCode == HTTP_CODE_OK) // 请求被服务器正常响应，等同于httpCode == 200
    {
      String payload = http.getString(); // 读取服务器返回的响应正文数据
                                         // 如果正文数据很多该方法会占用很大的内存
      // Serial.println(payload);
      // while(payload[i] != '\n') i ++;
      // i++;
      i = 10;
      while(p <= 6){
        while (payload[i] < '0' || payload[i] > '9') i++;
        while (payload[i] >= '0' && payload[i] <= '9'){
          dt[p] = (dt[p] * 10 + payload[i] - '0' ) % 100;
          i++;
        }
        p++;
      }
      clk.setTime(dt[0], dt[1], dt[2], dt[4], dt[5], dt[6], dt[3]);
      Serial.println("Setting Time Success...");
    }
  }
  else
  {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end(); // 结束当前连接
}


void webWifiConfig(){
  network->wifiConfig();
}