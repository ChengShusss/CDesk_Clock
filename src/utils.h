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

#define PROMPT_USER_COLOR 0x4c66
#define PROMPT_AT_COLOR 0x9492
#define PROMPT_HOST_COLOR 0x0973
#define PROMPT_MARK_COLOR 0xa31e

// =================================================
// String defination
// =================================================
#define FONT_FILE "/SansFont"

#define PROMPT_USER_TEXT "Shadow"
#define PROMPT_AT_SYMBOL " @ "
#define PROMPT_HOST_TEXT "esp32"
#define PROMPT_MARK_TEXT "$ >"
#define PROMPT_SPLIT_LINE "- - - - - - - - - - - - -"


#include <Ds1302.h>

void copyDateTime(Ds1302::DateTime* src, Ds1302::DateTime* dst);
void castTimeToSting(Ds1302::DateTime* time, char* dst);

uint16_t binarySerch(const uint16_t* arr, uint16_t size, uint16_t target);

#endif