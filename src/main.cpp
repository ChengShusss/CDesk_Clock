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

// Hardware widget init
Clock clk = Clock();
Bounce sw = Bounce();
// Software widget init
Display display = Display();
Network* network = new Network();

// Global varible init
MENU_STATE curState = INFO;
int16_t var_x;
int16_t var_y;
bool isConnected = false;
bool isMenuTyping = true;
uint8_t weatherIndex;
uint16_t updateWeatherCount = updateWeatherGap;
uint8_t weatherDisplay = 0;
unsigned char rocker_state = 0;
Ds1302::DateTime before;
Ds1302::DateTime now;
Weather todayWeather;
Weather tomorrowWeather;

// State Transform
//   Menu transform
KEY_TABLE menuTable[] = {
    {3, 1, 0, (*printTimeWeather)}, // 0: Info
    {0, 2, 1, (*printTime)}, // 1: Sync Info
    {1, 3, 2, (*testSync)}, // 2: Wifi Config
    {2, 0, 3, (*printTime)}, // 3: Reset
};
//   Info transform
INFO_TABLE infoTable[] = {
    {2, 1, (*printTimeWeather)}, // 0: Time
    {0, 2, (*printTime)}, // 1: Weather Today
    {1, 0, (*testSync)}, // 2: Weather Tomorrow
    {3, 3, (*printTime)}, // 3:config
};

const static char* WeatherInfos[] = {
  "Today",
  "Tonight",
  "Tomorrow",
  "Next Night"
};

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

const static char* WeatherTextsChs[] = {
    "晴",
    "晴",
    "晴",
    "晴",
    "多云",
    "多云",
    "多云",
    "多云",
    "多云",
    "阴",
    "阵雨",
    "雷阵雨",
    "阵雨冰雹",
    "小雨",
    "中雨",
    "大雨",
    "暴雨",
    "大暴雨",
    "特大暴雨",
    "冻雨",
    "雨夹雪",
    "阵雪",
    "小雪",
    "中雪",
    "大雪",
    "暴雪",
    "浮尘",
    "扬沙",
    "沙暴",
    "强沙暴",
    "雾",
    "霾",
    "风",
    "大风",
    "飓风",
    "热带风暴",
    "龙卷风",
    "冷",
    "热",
    "未知"
};


void setup()
{
    #ifdef DEBUG_CS
      delay(3000);
    #endif
    Serial.begin(115200);
    display.clean();

    sw.attach(PIN_SW, INPUT_PULLUP);
    sw.interval(DEBOUNCE_TIME);


    // TODO check here whether is funtional.
    if (digitalRead(PIN_SW) == LOW) {
      delay(1000);
      esp_wifi_restore();
      delay(10);
      ESP.restart();  //复位esp32
    }

    WiFi.begin();
    
    // test if clock is halted and set a date-time (see example 2) to start it
    if (clk.isHalted())
    {
        Serial.println("RTC is halted. Setting time...");
        clk.setTime(21, 9, 25, 21, 16, 30, 7);
    }

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
    
    clk.getTime(&before);
    clk.getTime(&now);

    display.clean();
    display.drawPrompt();
    menuState = INFO;
    
    updateMenu(true);
    printTime();
    updateWifiStatus(true);

    weatherIndex = 0;
    updateWeather(true);
}


void loop()
{
    network->handleHttpRequest();

    // Rocker update
    sw.update();
    rocker_state = scanRocker();
    if (rocker_state != 255){
        switch (rocker_state)
        {
          case 0: menuState = (MENU_STATE)menuTable[menuState].up; break;
          case 1: menuState = (MENU_STATE)menuTable[menuState].down; break;
        }
        updateMenu(true);
    }
    if(sw.fell()){
        curState = (MENU_STATE)menuTable[menuState].sw;
        (*menuTable[menuState].operation)();
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
  display.drawMenuItem(Menuitems[menuState], 0, isMenuTyping, refresh);
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
    if ((menuState == INFO && before.minute != now.minute)){
        printTime();
    }
    updateMenu(false);
    displayWeather();
}



// menu item info display entrance
void displayInfoMenu(void){

}

// menu item info sync entrance
void syncInfoMenu(void){


}

// menu item wifi config entrance
void configWifiMenu(void){


}

// menu item reset entrance
void resetMenu(void){


}


// print Info
void printInfo(bool force){
  
}