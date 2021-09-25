#include <clock.h>

#include <Ds1302.h>
#include "pin.h"

const static char* WeekDays[] =
{
    "Mon",
    "Tue",
    "Wed",
    "Thu",
    "Fri",
    "Sat",
    "Sun"
};

// DS1302 RTC instance


uint8_t parseDigits(char* str, uint8_t count)
{
    uint8_t val = 0;
    while(count-- > 0) val = (val * 10) + (*str++ - '0');
    return val;
}

Clock::Clock(void){
    this->rtc = &Ds1302(PIN_ENA, PIN_CLK, PIN_DAT);
    this->rtc->init();
}

Clock::Clock(int8_t pin_rst, int8_t pin_clk, int8_t pin_dat){
    this->rtc = &Ds1302(pin_rst, pin_clk, pin_dat);
    this->rtc->init();
}

Ds1302::DateTime Clock::getTime(void){
    Ds1302::DateTime now;
    this->rtc->getDateTime(&now);
    return now;
}
void Clock::setTime(uint8_t year, uint8_t month, uint8_t day, uint8_t dow, uint8_t hour,uint8_t minute, uint8_t second){
    Ds1302::DateTime dt;
    dt.year = year;
    dt.month = month;
    dt.day = day;
    dt.dow = dow;
    dt.hour = hour;
    dt.minute = minute;
    dt.second = second;
    this->rtc->setDateTime(&dt);
}

bool Clock::isHated(void){
    return this->rtc->isHalted();
}