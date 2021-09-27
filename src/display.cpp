#include "display.h"
#include "utils.h"

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

void Display::clean(void){
    this->tft.fillScreen(BACKGROUND);
}

void Display::showInitBar(int percent){
    this->tft.drawRoundRect(14, 60, 102, 8, 3, ST77XX_WHITE);
    this->tft.fillRoundRect(14 + percent, 61, 100 - percent, 6, 2, BACKGROUND);
    this->tft.fillRoundRect(14, 61, percent, 6, 2, ST77XX_WHITE);
    this->tft.setCursor(14, 72);
    this->tft.setTextColor(ST77XX_WHITE);
    this->tft.setTextSize(1);
    this->tft.print("Powered By Shadow");
}

void Display::drawTime(Ds1302::DateTime* before, Ds1302::DateTime* now){
    char time[] = "00:00";
    // set font size
    this->tft.setTextSize(4);

    // clean before time, avoid flicker.
    this->tft.setTextColor(BACKGROUND);
    this->tft.setCursor(6, 20);
    castTimeToSting(before, time);
    this->tft.print(time);
    
    // print time at tft
    this->tft.setTextColor(TIME_COLOR);
    this->tft.setCursor(6, 20);
    castTimeToSting(now, time);
    this->tft.print(time);
    

}


void Display::drawTime(Ds1302::DateTime* now){
    char time[] = "00:00";
    // set font size
    this->tft.setTextSize(4);
    // print time at tft
    this->tft.setTextColor(TIME_COLOR);
    this->tft.setCursor(6, 20);
    castTimeToSting(now, time);
    this->tft.print(time);

}

void Display::drawFrame(void){
    // draw horizontal lines
    this->tft.drawLine(0, 0, 128, 0, LINE_COLOR);
    this->tft.drawLine(0, 1, 128, 1, LINE_COLOR);
    this->tft.drawLine(0, 10, 128, 10, LINE_COLOR);
    this->tft.drawLine(0, 11, 128, 11, LINE_COLOR);
    this->tft.drawLine(0, 55, 128, 55, LINE_COLOR);
    this->tft.drawLine(0, 56, 128, 56, LINE_COLOR);
    this->tft.drawLine(0, 126, 128, 126, LINE_COLOR);
    this->tft.drawLine(0, 127, 128, 127, LINE_COLOR);

    // draw vertical lines
    this->tft.drawLine(0, 0, 0, 128, LINE_COLOR);
    this->tft.drawLine(1, 0, 1, 128, LINE_COLOR);
    this->tft.drawLine(96, 55, 96, 128, LINE_COLOR);
    this->tft.drawLine(97, 55, 97, 128, LINE_COLOR);
    this->tft.drawLine(126, 0, 126, 128, LINE_COLOR);
    this->tft.drawLine(127, 0, 127, 128, LINE_COLOR);
}

void Display::drawWifiStatus(char* status){

    this->tft.fillRect(2,0, 128, 8, BACKGROUND);
    // set font size
    this->tft.setTextSize(1);
    // print time at tft
    this->tft.setTextColor(TIME_COLOR);
    this->tft.setCursor(2, 2);
    this->tft.print(status);
}