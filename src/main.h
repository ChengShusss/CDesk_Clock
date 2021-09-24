#ifndef MAIN

#define MAIN
#include <Wire.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <esp_adc_cal.h>
#include <SPI.h>

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




#endif