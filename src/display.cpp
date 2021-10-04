#include "display.h"
#include "utils.h"
#include <SPIFFS.h>
#include "libs/fontData.h"
#include "libs/segment.h"
#include "libs/weatherIcons.h"



Display::Display(void):tft(TFT_CS, TFT_DC, TFT_RST){
    tft.initR(INITR_GREENTAB);      // Init ST7735S chip, green tab
    // TODO Need to change the following file: .pio\libdeps\esp32doit-devkit-v1\Adafruit ST7735 and ST7789 Library\Adafruit_ST7735.cpp:223
    //   _colstart = 0;
    //   _rowstart = 32;

    #ifdef DEBUG_CS
        Serial.println(F("Initialized"));
    #endif

    // Plot the frame.
    tft.fillScreen(ST77XX_BLUE);
      for(int16_t y = 0; y < 4; y++){
        tft.drawLine(0, y, WIDTH, y, ST77XX_WHITE);
    }
    for(int16_t y = 60; y < 64; y++){
        tft.drawLine(0, y, WIDTH, y, ST77XX_WHITE);
    }

    // Setting
    tft.setTextWrap(false);
};

void Display::print(int x, int y, char* text){
    tft.setCursor(x, y);
    tft.print(text);
};

void Display::print(int x, int y, char* text, uint16_t color, uint8_t size){
    tft.setTextSize(size);
    tft.setTextColor(color);
    tft.setCursor(x, y);
    tft.print(text);
};

void Display::println(int x, int y, char* text){
    tft.setCursor(x, y);
    tft.println(text);
};

void Display::println(int x, int y, char* text, uint16_t color, uint8_t size){
    tft.setTextSize(size);
    tft.setTextColor(color);
    tft.setCursor(x, y);
    tft.println(text);
};


void Display::drawBitmap(int x, int y, int w, int h, uint8_t* img, uint16_t color){
  tft.drawBitmap(x, y, img, w, h, color);
};

void Display::clean(void){
    tft.fillScreen(BACKGROUND);
}

void Display::showInitBar(int percent){
    tft.drawRoundRect(14, 60, 102, 8, 3, ST77XX_WHITE);
    tft.fillRoundRect(14 + percent, 61, 100 - percent, 6, 2, BACKGROUND);
    tft.fillRoundRect(14, 61, percent, 6, 2, ST77XX_WHITE);
    tft.setCursor(14, 72);
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(1);
    tft.print("Powered By Shadow");
}


void Display::drawPrompt(void){
  tft.setCursor(2, 4);
  tft.setTextSize(1);

  // print user
  tft.setTextColor(PROMPT_USER_COLOR);
  tft.print(PROMPT_USER_TEXT);
  // print @
  tft.setTextColor(PROMPT_AT_COLOR);
  tft.print(PROMPT_AT_SYMBOL);
  // print host
  tft.setTextColor(PROMPT_HOST_COLOR);
  tft.print(PROMPT_HOST_TEXT);

  // print prompt signal
  tft.setCursor(2, 14);
  // tft.setTextSize(2);
  tft.setTextColor(PROMPT_MARK_COLOR);
  tft.print(PROMPT_MARK_TEXT);

  // print prompt split line
  tft.setCursor(0, 20);
  // tft.setTextSize(2);
  tft.setTextColor(LINE_COLOR);
  tft.print(PROMPT_SPLIT_LINE);
}

void Display::drawTime(Ds1302::DateTime* before, Ds1302::DateTime* now){
    // char time[] = "00:00";
    // // set font size


    // // clean before time, avoid flicker.
    // this->tft.setTextColor(LINE_COLOR);
    // this->tft.setCursor(6, 64);
    // castTimeToSting(before, time);
    // this->tft.print("88888");
    
    // // print time at tft
    // this->tft.setTextColor(TIME_COLOR);
    // this->tft.setCursor(6, 64);
    // castTimeToSting(now, time);
    // this->tft.print(time);

    // tft.setFont();
    

}


void Display::drawTime(Ds1302::DateTime* now){

    uint16_t offset;

    // Draw background
    tft.drawBitmap(6, 28, segmentFontData + 960, 24, 40, LINE_COLOR);
    tft.drawBitmap(30, 28, segmentFontData + 960, 24, 40, LINE_COLOR);
    tft.drawBitmap(66, 28, segmentFontData + 960, 24, 40, LINE_COLOR);
    tft.drawBitmap(90, 28, segmentFontData + 960, 24, 40, LINE_COLOR);


    offset = segmentIndexTable[now->hour / 10];
    // Draw Hours
    tft.drawBitmap(6, 28, segmentFontData + offset, 24, 40, TIME_COLOR);
    offset = segmentIndexTable[now->hour % 10];
    tft.drawBitmap(30, 28, segmentFontData + offset, 24, 40, TIME_COLOR);
    // Draw Minutes
    offset = segmentIndexTable[now->minute / 10];
    tft.drawBitmap(66, 28, segmentFontData + offset, 24, 40, TIME_COLOR);
    offset = segmentIndexTable[now->minute % 10];
    tft.drawBitmap(90, 28, segmentFontData + offset, 24, 40, TIME_COLOR);

}

void Display::drawMenuItem(const char* menuItems, unsigned char n, bool isTyping){
  tft.setCursor(20, 14);
  tft.setTextSize(1);
  tft.setTextColor(PROMPT_COMMAND_COLOR);
  tft.print(menuItems);
  if(!isTyping){
    tft.setTextColor(BACKGROUND);
  }
  tft.print('_');
}


void Display::drawWifiStatus(const char* status){

    tft.fillRect(2,120, 128, 8, BACKGROUND);
    // set font size
    tft.setTextSize(1);
    // print time at tft
    tft.setTextColor(TIME_COLOR);
    tft.setCursor(2, 120);
    tft.print(status);
}

void Display::drawWeater(uint8_t index, bool isDay){
  tft.fillRect(2, 80, 32, 32, BACKGROUND);
  tft.drawBitmap(2, 80, weatherIcons + index * 128, 32, 32, TIME_COLOR);
}

// void Display::openFontFile(void){
//   static File f = SPIFFS.open(FONT_FILE);
//   fontFile = &f;
// }

int Display::drawUtf8Char(uint16_t uni, uint8_t x, uint8_t y, uint16_t color){
  uint16_t offset = binarySerch(indexTable, 3606, uni);
  if (offset == 65535){
    tft.drawBitmap(x, y, unFind, 16, 15, color);
  } else{
    tft.drawBitmap(x, y, fontData + (offset * 30), 16, 15, color);
  }
  
  return 0;
}

void Display::drawUtf8String(const char* utf8Str, uint8_t x, uint8_t y, uint16_t color){
  uint8_t cur_x = x;
  uint8_t p = 0;
  uint16_t unicode = 0;
  while (*(utf8Str + p)){
    unicode = 0;
    uint8_t byte1 = *(utf8Str + p);
    p++;
    if ((byte1 & 0x80) == 0) {
      unicode = byte1;
    } else {
      uint8_t byte2 = *(utf8Str + p);
      p++;
      if (byte2 == 0) {
        break;
      }
      if ((byte1 & 0xE0) == 0xC0) {
        unicode = ((byte1 & 0x1F) << 6) | (byte2 & 0x3F);
      } else {
        uint8_t byte3 = *(utf8Str + p);
        p++;
        if (byte3 == 0) {
          break;
        }
        if ((byte1 & 0xf0) == 0xE0) {
          unicode =
              ((byte1 & 0x0F) << 12) | ((byte2 & 0x3F) << 6) | (byte3 & 0x3F);
        } else {
          break;
        }
      }
    }
    drawUtf8Char(unicode, cur_x, y, color);
    cur_x += 16;
  }
}