#ifndef _DISPLAY_

#define _DISPLAY_
#include <Wire.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPIFFS.h>
#include <SPI.h>
#include <Ds1302.h>
#include "pin.h"
#include "utils.h"

// Debug Mode
#define DEBUG_CS


#define WIDTH 128
#define HEIGHT 128

#define FONT_CHINESE_WIDTH 16
#define FONT_CHINESE_HEIGHT 15
#define FONT_CHAR_SIZE 31
#define FONT_PAGE_SIZE 7936

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

    void drawBitmap(int x, int y, int w, int h, uint8_t* img, uint16_t color);

    void clean(void);
    void showInitBar(int Percent);

    void drawPrompt(void);

    void drawTime(Ds1302::DateTime* before, Ds1302::DateTime* now);
    void drawTime(Ds1302::DateTime* now);

    // void drawFrame(void);

    void drawMenuItem(const char* menuItems, unsigned char n, bool isTyping);
    
    void drawWifiStatus(const char* status);

    
    // void openFontFile(void);
    int drawUtf8Char(uint16_t uni, uint8_t x, uint8_t y, uint16_t color);

    void drawUtf8String(const char* utf8Str, uint8_t x, uint8_t y, uint16_t color);
// private:
    // For 1.44" and 1.8" TFT with ST7735 use:
    Adafruit_ST7735 tft;

    // File*  fontFile;

};

#endif