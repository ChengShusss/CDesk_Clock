#include "main.h"
#include "display.h"

int minute = 50;
int hour = 9;
int AN_x = 0;
int AN_y = 0;
char times[] = "aaaaa";


Display display = Display();

void formatTime(char* , int, int);


void setup(void) {
  #ifdef DEBUG_CS
    Serial.begin(9600);
    
    delay(3000);
    Serial.println(F("Hello! ST77xx TFT Test"));
  #endif

  #ifdef DEBUG_CS
    Serial.println(F("Initialized"));
    Serial.println("Setup stage done.");
    delay(1000);
  #endif
}

void loop() {
  display.print(4, 18, times, ST77XX_BLUE, 4);

  // update time.
  if (minute < 59){
    minute++;
  }else{
    minute = 0;
    if (hour < 23){
      hour++;
    }else{
      hour = 0;
    }
  }
  formatTime(times, hour, minute);
  display.print(4, 18, times, ST77XX_ORANGE, 4);
  delay(500);
  display.print(52, 18, ":", ST77XX_BLUE, 4);
  delay(500);

  AN_x = analogRead(VR_X);
  AN_y = analogRead(VR_Y);
  #ifdef DEBUG_CS
    Serial.print(F("X: "));
    Serial.print(AN_x >> 5);
    Serial.print(F("; Y: "));
    Serial.println(AN_y >> 5);
  #endif

}

// void testlines(uint16_t color) {
//   tft.fillScreen(ST77XX_BLACK);
//   for (int16_t x=0; x < tft.width(); x+=6) {
//     tft.drawLine(0, 0, x, tft.height()-32, color);
//     delay(0);
//   }
//   for (int16_t y=0; y < tft.height() - 32; y+=6) {
//     tft.drawLine(0, 0, tft.width()-1, y, color);
//     delay(0);
//   }

//   tft.fillScreen(ST77XX_BLACK);
//   for (int16_t x=0; x < tft.width(); x+=6) {
//     tft.drawLine(tft.width()-1, 0, x, tft.height()-32, color);
//     delay(0);
//   }
//   for (int16_t y=0; y < tft.height() - 32; y+=6) {
//     tft.drawLine(tft.width()-1, 0, 0, y, color);
//     delay(0);
//   }

//   tft.fillScreen(ST77XX_BLACK);
//   for (int16_t x=0; x < tft.width(); x+=6) {
//     tft.drawLine(0, tft.height()- 32, x, 0, color);
//     delay(0);
//   }
//   for (int16_t y=0; y < tft.height(); y+=6) {
//     tft.drawLine(0, tft.height()- 32, tft.width()-1, y, color);
//     delay(0);
//   }

//   tft.fillScreen(ST77XX_BLACK);
//   for (int16_t x=0; x < tft.width(); x+=6) {
//     tft.drawLine(tft.width()-1, tft.height()- 32, x, 0, color);
//     delay(0);
//   }
//   for (int16_t y=0; y < tft.height()- 32; y+=6) {
//     tft.drawLine(tft.width()-1, tft.height()- 32, 0, y, color);
//     delay(0);
//   }
// }

// void testdrawtext(char *text, uint16_t color) {
//   tft.setCursor(0, 0);
//   tft.setTextColor(color);
//   tft.setTextWrap(true);
//   tft.print(text);
// }

// void testfastlines(uint16_t color1, uint16_t color2) {
//   tft.fillScreen(ST77XX_BLACK);
//   for (int16_t y=0; y < tft.height()- 32; y+=5) {
//     tft.drawFastHLine(0, y, tft.width(), color1);
//   }
//   for (int16_t x=0; x < tft.width(); x+=5) {
//     tft.drawFastVLine(x, 0, tft.height()- 32, color2);
//   }
// }

// void testdrawrects(uint16_t color) {
//   tft.fillScreen(ST77XX_BLACK);
//   for (int16_t x=0; x < tft.width(); x+=6) {
//     tft.drawRect(tft.width()/2 -x/2, (tft.height()- 32)/2 -x/2 , x, x, color);
//   }
// }

// void testfillrects(uint16_t color1, uint16_t color2) {
//   tft.fillScreen(ST77XX_BLACK);
//   for (int16_t x=tft.width()-1; x > 6; x-=6) {
//     tft.fillRect(tft.width()/2 -x/2, (tft.height()- 32)/2 -x/2 , x, x, color1);
//     tft.drawRect(tft.width()/2 -x/2, (tft.height()- 32)/2 -x/2 , x, x, color2);
//   }
// }

// void testfillcircles(uint8_t radius, uint16_t color) {
//   for (int16_t x=radius; x < tft.width(); x+=radius*2) {
//     for (int16_t y=radius; y < (tft.height()- 32); y+=radius*2) {
//       tft.fillCircle(x, y, radius, color);
//     }
//   }
// }

// void testdrawcircles(uint8_t radius, uint16_t color) {
//   for (int16_t x=0; x < tft.width()+radius; x+=radius*2) {
//     for (int16_t y=0; y < (tft.height()- 32)+radius; y+=radius*2) {
//       tft.drawCircle(x, y, radius, color);
//     }
//   }
// }

// void testtriangles() {
//   tft.fillScreen(ST77XX_BLACK);
//   uint16_t color = 0xF800;
//   int t;
//   int w = tft.width()/2;
//   int x = tft.height()-32;
//   int y = 0;
//   int z = tft.width();
//   for(t = 0 ; t <= 15; t++) {
//     tft.drawTriangle(w, y, y, x, z, x, color);
//     x-=4;
//     y+=4;
//     z-=4;
//     color+=100;
//   }
// }

// void testroundrects() {
//   tft.fillScreen(ST77XX_BLACK);
//   uint16_t color = 100;
//   int i;
//   int t;
//   for(t = 0 ; t <= 4; t+=1) {
//     int x = 0;
//     int y = 0;
//     int w = tft.width()-2;
//     int h = tft.height()-34;
//     for(i = 0 ; i <= 16; i+=1) {
//       tft.drawRoundRect(x, y, w, h, 5, color);
//       x+=2;
//       y+=3;
//       w-=4;
//       h-=6;
//       color+=1100;
//     }
//     color+=100;
//   }
// }

// void tftPrintTest() {
//   tft.setTextWrap(false);
//   tft.fillScreen(ST77XX_BLACK);
//   tft.setCursor(0, 0);
//   tft.setTextColor(ST77XX_RED);
//   tft.setTextSize(1);
//   tft.println("Hello World!");
//   tft.setTextColor(ST77XX_YELLOW);
//   tft.setTextSize(2);
//   tft.println("Hello World!");
//   tft.setTextColor(ST77XX_GREEN);
//   tft.setTextSize(3);
//   tft.println("Hello World!");
//   tft.setTextColor(ST77XX_BLUE);
//   tft.setTextSize(4);
//   tft.print(1234.567);
//   delay(1500);
//   tft.setCursor(0, 0);
//   tft.fillScreen(ST77XX_BLACK);
//   tft.setTextColor(ST77XX_WHITE);
//   tft.setTextSize(0);
//   tft.println("Hello World!");
//   tft.println(" ");
//   tft.print(8675309, HEX); // print 8,675,309 out in HEX!
//   tft.println(" Print HEX!");
//   tft.println(" ");
//   tft.setTextColor(ST77XX_WHITE);
//   tft.println("Sketch has been");
//   tft.println("running for: ");
//   tft.setTextColor(ST77XX_MAGENTA);
//   tft.print(millis() / 1000);
//   tft.setTextColor(ST77XX_WHITE);
//   tft.print(" seconds.");
// }

// void mediabuttons() {
//   // play
//   tft.fillScreen(ST77XX_BLACK);
//   tft.fillRoundRect(25, 10, 78, 60, 8, ST77XX_WHITE);
//   tft.fillTriangle(42, 20, 42, 60, 90, 40, ST77XX_RED);
//   delay(500);
//   // pause
//   tft.fillRoundRect(25, 90, 78, 60, 8, ST77XX_WHITE);
//   tft.fillRoundRect(39, 98, 20, 45, 5, ST77XX_GREEN);
//   tft.fillRoundRect(69, 98, 20, 45, 5, ST77XX_GREEN);
//   delay(500);
//   // play color
//   tft.fillTriangle(42, 20, 42, 60, 90, 40, ST77XX_BLUE);
//   delay(50);
//   // pause color
//   tft.fillRoundRect(39, 98, 20, 45, 5, ST77XX_RED);
//   tft.fillRoundRect(69, 98, 20, 45, 5, ST77XX_RED);
//   // play color
//   tft.fillTriangle(42, 20, 42, 60, 90, 40, ST77XX_GREEN);
// }


void formatTime(char formated[], int hour, int minute){
  formated[0] = (hour % 100 / 10) + '0';
  formated[1] = (hour % 10) + '0';
  formated[2] = ':';
  formated[3] = (minute % 100 / 10) + '0';
  formated[4] = (minute % 10) + '0';
}