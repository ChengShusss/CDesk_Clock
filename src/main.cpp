#include <Arduino.h>
#include <Bounce2.h>
#include <WiFi.h>
#include <HTTPClient.h>

#include "clock.h"
#include "main.h"
#include "display.h"
#include "utils.h"
#include "network.h"
#include "weather.h"

#define DEBOUNCE_TIME 10 //延时用来过滤不正常的信号，

Clock clk = Clock();
Display display = Display();
Bounce sw = Bounce();

Ds1302::DateTime before;
Ds1302::DateTime now;
int8_t state_t = 0;
int16_t var_x;
int16_t var_y;
bool isConnected = false;
bool isMenuTyping = true;
WiFiClient wifi_Client;
HTTPClient http_client;
Network* network = new Network();
String req;
String rsp;


uint8_t weatherIndex;
Weather todayWeather;
Weather tomorrowWeather;
uint16_t updateWeatherCount = updateWeatherGap;
uint8_t weatherDisplay = 0;

const static char* WeatherInfos[] = {
  "Today",
  "Tonight",
  "Tomorrow",
  "Next Night"
};

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

const static char* WifiStatusText[] = {
  "Offline",
  "Connect"
};

const static char* WeatherTexts[] = {
    "Sunny",
    "Clear",
    "Fair",
    "Fair",
    "Cloudy",
    "Few Cloudy",
    "Few Cloudy",
    "Most-Cloudy",
    "Most-Cloudy",
    "Overcast",
    "Shower",
    "Thun-shower",
    "Thund-Hail",
    "Light Rain",
    "Mid-Rain",
    "Heavy Rain",
    "Storm",
    "Heavy Storm",
    "Severe Storm",
    "Ice Rain",
    "Sleet",
    "Snow Flurry",
    "Light Snow",
    "Mid-Snow",
    "Heavy Snow",
    "Snowstorm",
    "Dust",
    "Sand",
    "Duststorm",
    "Sandstorm",
    "Foggy",
    "Haze",
    "Windy",
    "Blustery",
    "Hurricane",
    "Tropi-Storm",
    "Tornado",
    "Cold",
    "Hot",
    "Unknown"
};

KEY_TABLE menuTable[] = {
    {0, 0, 0, 1, 1, (*printTime)}, // 0:normal
    {3, 2, 0, 4, 4, (*printTime)}, // 1:time set
    {1, 3, 0, 6, 6, (*printTime)}, // 2:wifi set
    {2, 1, 0, 3, 3, (*printTime)}, // 3:about
    {5, 5, 1, 4, 4, (*testSync)}, // 4:auto sync
    {4, 4, 1, 5, 5, (*printTime)}, // 5:manual set
    {7, 7, 2, 6, 6, (*webWifiConfig)}, // 6:connect
    {6, 6, 2, 7, 7, (*printTime)}  // 7:reset
};

uint8_t noteIndex = 0;

const static char testChinese[] = "测试中文";
unsigned indexChinese = 0;



void setup()
{
  
    Serial.begin(115200);
    delay(3000);
  
    // SPIFFS.begin();
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
    // display.drawFrame();
    // printMenu();
    // display.openFontFile();
    
    // display.tft.fillRect(2, 4, 124, 18, BACKGROUND);
    display.drawPrompt();
    state = DATETIME;
    updateMenu();
    updateWifiStatus(true);
    weatherIndex = 0;
}


void loop()
{
    network->handleHttpRequest();
    // get the current time

    // Rocker update
    sw.update();
    rocker_state = scanRocker();
    if (rocker_state != 255){
        // state = menuJump[state][rocker_state];
        switch (rocker_state)
        {
          case 0: state_t = menuTable[state_t].up; break;
          case 1: state_t = menuTable[state_t].down; break;
          case 2: state_t = menuTable[state_t].left; break;
          case 3: state_t = menuTable[state_t].right; break;
        }
        // printMenu();
    }
    if(sw.fell()){
        state_t = menuTable[state_t].sw;
        (*menuTable[state_t].operation)();
    }

    clk.getTime(&now);
    if (before.second != now.second){
      if (state == DATETIME && before.minute != now.minute){
        printTime();
      }
      updateWifiStatus(false);
      copyDateTime(&now, &before);

      // display.tft.fillRect(6, 88, 124, 16, BACKGROUND);
      // // display.drawUtf8Char(0x62c9, 6, 88, ST7735_CYAN);
      // display.drawUtf8String(Notes[0], 6, 88, ST7735_CYAN);

      updateMenu();
      // printInfo();
      // updateWeather();
      updateWeather();
      displayWeather();
      
      weatherDisplay += 1;
    }

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
    Serial.println("Print Info: ");

    #endif
}

void updateMenu(void){
  display.drawMenuItem(Menuitems[state], 0, isMenuTyping);
  isMenuTyping = !isMenuTyping;
}

void updateWifiStatus(bool force){
  if (force | (isConnected != network->isConnected())){
    isConnected = network->isConnected();
    display.drawWifiStatus(WifiStatusText[isConnected]);
  }
}


void printTime(void){
    display.drawTime(&now);
}

void testSync(void){
  network->syncTime(&clk);
}


void webWifiConfig(){
  network->wifiConfig();
}

void displayWeather(void){
  static uint8_t cur_opt = 128;
  if( weatherDisplay >> 2 != cur_opt){
    Serial.print("cur_opt");
    Serial.println(cur_opt);
    display.drawWeather(
      &todayWeather, &tomorrowWeather,
      (weatherDisplay & 0x1f) >> 2,
      WeatherInfos,
      WeatherTexts);
    cur_opt = weatherDisplay >> 2;
  }
}

void updateWeather(void){
  if (updateWeatherCount >= updateWeatherGap){
    network->getWeather(&todayWeather, &tomorrowWeather);
    // printWeather(&todayWeather);
    updateWeatherCount = 0;
  }else{
    updateWeatherCount += 1;
  }
  
}