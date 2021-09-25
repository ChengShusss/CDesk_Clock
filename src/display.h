#ifndef _DISPLAY_

#define _DISPLAY_
#include <Wire.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
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


class Display{
public:
    Display(void);
    // void init(void);

    void print(int x, int y, char* text);
    void print(int x, int y, char* text, uint16_t color, uint8_t size);

    void println(int x, int y, char* text);
    void println(int x, int y, char* text, uint16_t color, uint8_t size);
private:
    // For 1.44" and 1.8" TFT with ST7735 use:
    Adafruit_ST7735 tft;
};

#endif