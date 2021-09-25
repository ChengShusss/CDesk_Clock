#ifndef _CLOCK_CDC
#define _CLOCK_CDC

#include <Ds1302.h>

class Clock{
private:
    Ds1302 rtc;
    Ds1302::DateTime datatime;

public:
    Clock(void);
    void getTime(Ds1302::DateTime*);
    void setTime(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second, uint8_t dow);
    void setTime(Ds1302::DateTime*);
    bool isHalted(void);
};

#endif