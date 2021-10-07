#ifndef MAIN

#define MAIN
#include "display.h"
#include "pin.h"

// Debug Mode
#define DEBUG_CS


#define updateWeatherGap 15


#define VR_Y 34
#define VR_X 35


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

// 定义状态的枚举
enum STATE{
  DATETIME,
  SET_TIME,
  SYNC_TIME,
  SYNC_WEATHER,
  SET_WIFI,
  RESET
} state;


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


#endif