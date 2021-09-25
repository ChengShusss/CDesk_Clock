#ifndef MAIN

#define MAIN
#include "display.h"

// Debug Mode
#define DEBUG_CS

// Pin define
#define TFT_CS         14
#define TFT_RST        2
#define TFT_DC         4
#define TFT_BACKLIGHT  15
#define TFT_MOSI 23  // Data out
#define TFT_SCLK 18  // Clock out

#define WIDTH 128
#define HEIGHT 128

#define VR_Y 34
#define VR_X 35

void wifi_Config();

//用于上电自动连接WiFi
bool AutoConfig();


#endif