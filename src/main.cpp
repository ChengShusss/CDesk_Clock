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
STATE curState = DATETIME;
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
    "date time",
    "set time",
    "sync time",
    "sync weather",
    "set wifi",
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
    {5, 2, 0, 0, 0, (*printTimeWeather)}, // 0:normal
    {0, 2, 1, 1, 1, (*printTime)}, // 1:set time
    {1, 3, 2, 2, 2, (*testSync)}, // 2:sync time
    {2, 4, 3, 3, 3, (*printTime)}, // 3:sync weather
    {3, 5, 4, 4, 4, (*testSync)}, // 4:set wifi
    {4, 0, 5, 5, 5, (*printTime)}, // 5:reset
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
        delay(500);
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
    updateMenu(true);
    updateWifiStatus(true);

    weatherIndex = 0;
    updateWeather(true);
}


void loop()
{
  // handle http request to config wifi.
    network->handleHttpRequest();

    // Rocker update
    sw.update();
    rocker_state = scanRocker();
    if (rocker_state != 255){
        switch (rocker_state)
        {
          case 0: state = (STATE)menuTable[state].up; break;
          case 1: state = (STATE)menuTable[state].down; break;
          case 2: state = (STATE)menuTable[state].left; break;
          case 3: state = (STATE)menuTable[state].right; break;
        }
        updateMenu(true);
    }
    if(sw.fell()){
        curState = (STATE)menuTable[state].sw;
        (*menuTable[state].operation)();
    }

    clk.getTime(&now);
    if (before.second != now.second){
      if ((before.minute) != now.minute){
        updateWeather(false);
      }
      
      printTimeWeather();

      updateWifiStatus(false);
      copyDateTime(&now, &before);
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

void updateMenu(bool refresh){
  display.drawMenuItem(Menuitems[state], 0, isMenuTyping, refresh);
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
  bool res = network->syncTime(&clk);
  display.tft.fillRect(30, 56, 68, 20, ST7735_YELLOW);
  display.tft.setCursor(32, 60);
  display.tft.setTextColor(ST7735_BLACK);
  if(res){
    display.tft.print("Success");
  }else{
    display.tft.print("Failed");
  }
  delay(3000);
  initScreen();
  displayWeather();
  printTime();
  updateWifiStatus(true);
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
  weatherDisplay++;
}

void updateWeather(bool force){
  if (force | (updateWeatherCount >= updateWeatherGap)){
    network->getWeather(&todayWeather, &tomorrowWeather);
    updateWeatherCount = 0;
  }else{
    updateWeatherCount += 1;
  }
}


void initScreen(void){
  display.clean();
  display.drawPrompt();
  updateMenu(true);
}

void printTimeWeather(){
    if ((state == DATETIME && before.minute != now.minute)){
        printTime();
    }
    updateMenu(false);
    displayWeather();
}

void setTime(){

}
void syncTime(){

}
void syncWeather(){

}
void setWifi(){

}
void reset(){

}