#ifndef MAIN

#define MAIN
#include "display.h"
#include "pin.h"

// Debug Mode
#define DEBUG_CS


#define VR_Y 34
#define VR_X 35

void wifi_Config();

//用于上电自动连接WiFi
bool AutoConfig();

unsigned char scanRocker(void);

void printInfo(void);
void printTime(void);
void printMenu(void);


#endif