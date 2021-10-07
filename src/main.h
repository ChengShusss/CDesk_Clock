#ifndef MAIN

#define MAIN
#include "display.h"
#include "pin.h"

// Debug Mode
#define DEBUG_CS

#define DEBOUNCE_TIME 10 //延时用来过滤不正常的信号，


#define updateWeatherGap 15


#define VR_Y 34
#define VR_X 35


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


//多级菜单相关变量, 定义按键结构体
typedef struct
{
  unsigned char up;
  unsigned char down;
  unsigned char sw;
  void (*operation)(void);
} KEY_TABLE;

typedef struct
{
  unsigned char left;
  unsigned char right;
  void (*operation)(void);
} INFO_TABLE;

// Menu State
enum MENU_STATE{
  INFO,
  INFO_SYNC,
  WIFI_CONFIG,
  RESET
} menuState;

// Info State
enum INFO_STATE{
  TIME,
  TODAY_WEATHER,
  TOMORROW_WEATHER
};

void wifi_Config();

//用于上电自动连接WiFi
bool AutoConfig();

unsigned char scanRocker(void);

void printInfo(void);


void updateMenu(bool refresh);
void updateWifiStatus(bool force);
void printTime(void);

void testSync(void);

void webWifiConfig();

void displayWeather(void);
void updateWeather(bool force);

void initScreen(void);


void printTimeWeather(void);
void reset();

void displayInfoMenu(void);
void syncInfoMenu(void);
void configWifiMenu(void);
void resetMenu(void);

void printInfo(bool force);


#endif