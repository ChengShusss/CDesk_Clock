#ifndef _CLOCK_CDC
#define _CLOCK_CDC

#include <Ds1302.h>



class Clock{
private:
    Ds1302* rtc;

public:
    Clock(int8_t, int8_t, int8_t);
    Ds1302::DateTime getTime(void);
    void setTime(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t,uint8_t, uint8_t);
};

#endif