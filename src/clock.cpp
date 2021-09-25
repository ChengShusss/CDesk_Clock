#include <clock.h>

#include <Ds1302.h>
#include "pin.h"


Clock::Clock():rtc(PIN_ENA, PIN_CLK, PIN_DAT){
    this->rtc.init();
}

void Clock::getTime(Ds1302::DateTime* now){
    this->rtc.getDateTime(now);
}

void Clock::setTime(uint8_t year, uint8_t month, uint8_t day, uint8_t hour,uint8_t minute, uint8_t second, uint8_t dow){
    Ds1302::DateTime dt;
    dt.year = year;
    dt.month = month;
    dt.day = day;
    dt.hour = hour;
    dt.minute = minute;
    dt.second = second;
    dt.dow = dow;
    this->rtc.setDateTime(&dt);
}

void Clock::setTime(Ds1302::DateTime* dt){
    this->rtc.setDateTime(dt);
}

bool Clock::isHalted(void){
    return this->rtc.isHalted();
}