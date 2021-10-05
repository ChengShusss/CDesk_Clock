#include "display.h"
#include "utils.h"
#include <SPIFFS.h>
#include "libs/fontData.h"
#include "libs/segment.h"
#include "libs/weatherIcons.h"



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


void Display::drawBitmap(int x, int y, int w, int h, uint8_t* img, uint16_t color){
  tft.drawBitmap(x, y, img, w, h, color);
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
    tft.drawBitmap(TIME_TEXT_LEFT, TIME_TEXT_UP, segmentFontData + 960, 24, 40, LINE_COLOR);
    tft.drawBitmap(TIME_TEXT_LEFT + 24, TIME_TEXT_UP, segmentFontData + 960, 24, 40, LINE_COLOR);
    tft.drawBitmap(TIME_TEXT_LEFT + 60, TIME_TEXT_UP, segmentFontData + 960, 24, 40, LINE_COLOR);
    tft.drawBitmap(TIME_TEXT_LEFT + 84, TIME_TEXT_UP, segmentFontData + 960, 24, 40, LINE_COLOR);


    offset = segmentIndexTable[now->hour / 10];
    // Draw Hours
    tft.drawBitmap(TIME_TEXT_LEFT, TIME_TEXT_UP, segmentFontData + offset, 24, 40, TIME_COLOR);
    offset = segmentIndexTable[now->hour % 10];
    tft.drawBitmap(TIME_TEXT_LEFT + 24, TIME_TEXT_UP, segmentFontData + offset, 24, 40, TIME_COLOR);
    // Draw Minutes
    offset = segmentIndexTable[now->minute / 10];
    tft.drawBitmap(TIME_TEXT_LEFT + 60, TIME_TEXT_UP, segmentFontData + offset, 24, 40, TIME_COLOR);
    offset = segmentIndexTable[now->minute % 10];
    tft.drawBitmap(TIME_TEXT_LEFT + 84, TIME_TEXT_UP, segmentFontData + offset, 24, 40, TIME_COLOR);

}

void Display::drawMenuItem(const char* menuItems, unsigned char n, bool isTyping){
  tft.setCursor(20, 14);
  tft.setTextSize(1);
  tft.setTextColor(PROMPT_USER_COLOR);
  tft.print(menuItems);
  if(!isTyping){
    tft.setTextColor(BACKGROUND);
  }
  tft.print('_');
}


void Display::drawWifiStatus(const char* status){

    this->tft.fillRect(2,120, 128, 8, BACKGROUND);
    // set font size
    this->tft.setTextSize(1);
    // print time at tft
    this->tft.setTextColor(TIME_COLOR);
    this->tft.setCursor(2, 120);
    this->tft.print(status);
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


void Display::drawWeather(
  Weather* today, 
  Weather* tomorrow, 
  uint8_t option,
  const char** header, 
  const char** text){
    // =======================================
    uint16_t offset;
    Weather* w;
    uint8_t code;

    // Choose today or tomorrow
    if (!(option&0x04)){
      w = today;
    }else{
      w = tomorrow;
    }

    // Choose daylight or night
    if (!(option&0x02)){
      code = w->dayCode;
    }else{
      code = w->nightCode;
    }

    tft.fillRect(2, WEATHER_TEXT_UP, 124, 50, BACKGROUND);

    if (code > 38){
      offset = 69 * 128;
    }else{
      offset = weatherIndex[code] * 128;
    }
    // draw Icon
    tft.drawBitmap(94, WEATHER_TEXT_UP + 10, weatherIcons + offset, 32, 32, TIME_COLOR);

    // Display detail info
    tft.setTextSize(1);
    tft.setTextColor(TIME_COLOR);
    
    tft.setCursor(WEAHTER_TEXT_LEFT, WEATHER_TEXT_UP);
    tft.print(header[option >> 1]);
    tft.setCursor(WEAHTER_TEXT_LEFT, WEATHER_TEXT_UP + 10);
    if (code < 39){
      tft.print(text[code]);
    }else{
      tft.print(text[39]);
    }
    

    if (!(option & 0x01)){
      tft.setCursor(WEAHTER_TEXT_LEFT, WEATHER_TEXT_UP + 20);
      tft.print("Temp ");
      tft.print(w->lowTemp);
      tft.print('-');
      tft.print(w->highTemp);
      tft.print("`C");
      tft.setCursor(WEAHTER_TEXT_LEFT, WEATHER_TEXT_UP + 30);
      tft.print("Rain ");
      tft.print(w->precip);
      tft.print("%");
      tft.setCursor(WEAHTER_TEXT_LEFT, WEATHER_TEXT_UP + 40);
      tft.print("     ");
      tft.print(w->rainFall);
      tft.print(" mm");
    }else{
      tft.setCursor(WEAHTER_TEXT_LEFT, WEATHER_TEXT_UP + 20);
      tft.print("Wind rank ");
      tft.print(w->windRank);
      tft.print(" ");
      tft.print(w->windDirection);

      tft.setCursor(WEAHTER_TEXT_LEFT, WEATHER_TEXT_UP + 30);
      tft.print("  ");
      tft.print(w->windSpeed);
      tft.print(" kmph");
      tft.setCursor(WEAHTER_TEXT_LEFT, WEATHER_TEXT_UP + 40);
      tft.print("Humidity ");
      tft.print(w->humidity);
      tft.print(" %");
    }
  
}