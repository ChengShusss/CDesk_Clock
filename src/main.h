#ifndef MAIN

#define MAIN
#include "display.h"
#include "pin.h"

// Debug Mode
#define DEBUG_CS


#define VR_Y 34
#define VR_X 35


//多级菜单相关变量, 定义按键结构体
typedef struct
{
  unsigned char up;
  unsigned char down;
  unsigned char left;
  unsigned char right;
  unsigned char sw;
  void (*operation)(void);
} KEY_TABLE;


void wifi_Config();

//用于上电自动连接WiFi
bool AutoConfig();

unsigned char scanRocker(void);

void printInfo(void);
void printTime(void);
void printMenu(void);


void testAbout(void);

void testSync(void);


#endif