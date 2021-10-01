#ifndef _UTILS_CDC
#define _UTILS_CDC

// =================================================
// COLOR defination
// =================================================

// Bellow is copied from Library Adafruit.
// #define ST77XX_BLACK 0x0000
// #define ST77XX_WHITE 0xFFFF
// #define ST77XX_RED 0xF800
// #define ST77XX_GREEN 0x07E0
// #define ST77XX_BLUE 0x001F
// #define ST77XX_CYAN 0x07FF
// #define ST77XX_MAGENTA 0xF81F
// #define ST77XX_YELLOW 0xFFE0
// #define ST77XX_ORANGE 0xFC00
#define BACKGROUND 0x0000
#define TIME_COLOR 0xFFFF
#define LINE_COLOR 0x1042

#include <Ds1302.h>

void copyDateTime(Ds1302::DateTime* src, Ds1302::DateTime* dst);
void castTimeToSting(Ds1302::DateTime* time, char* dst);

#endif