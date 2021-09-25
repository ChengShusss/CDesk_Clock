#include "display.h"

Display::Display(void):tft(TFT_CS, TFT_DC, TFT_RST){
    this->tft.initR(INITR_GREENTAB);      // Init ST7735S chip, green tab
    // TODO Need to change the following file: .pio\libdeps\esp32doit-devkit-v1\Adafruit ST7735 and ST7789 Library\Adafruit_ST7735.cpp:223
    //   _colstart = 0;
    //   _rowstart = 32;

    #ifdef DEBUG_CS
        Serial.println(F("Initialized"));
    #endif

    // Plot the frame.
    this->tft.fillScreen(ST77XX_BLUE);
      for(int16_t y = 0; y < 4; y++){
        this->tft.drawLine(0, y, WIDTH, y, ST77XX_WHITE);
    }
    for(int16_t y = 60; y < 64; y++){
        this->tft.drawLine(0, y, WIDTH, y, ST77XX_WHITE);
    }

    // Setting
    this->tft.setTextWrap(false);
};

void Display::print(int x, int y, char* text){
    this->tft.setCursor(x, y);
    this->tft.print(text);
};

void Display::print(int x, int y, char* text, uint16_t color, uint8_t size){
    this->tft.setTextSize(size);
    this->tft.setTextColor(color);
    this->tft.setCursor(x, y);
    this->tft.print(text);
};

void Display::println(int x, int y, char* text){
    this->tft.setCursor(x, y);
    this->tft.println(text);
};

void Display::println(int x, int y, char* text, uint16_t color, uint8_t size){
    this->tft.setTextSize(size);
    this->tft.setTextColor(color);
    this->tft.setCursor(x, y);
    this->tft.println(text);
};