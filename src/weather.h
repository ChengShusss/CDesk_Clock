#ifndef _WEATHER_CDC
#define _WEATHER_CDC


#include <Arduino.h>


struct Weather {
    char date[11];
    uint8_t dayCode;
    uint8_t nightCode;
    int8_t highTemp;
    int8_t lowTemp;
    float rainFall;
    uint8_t precip;
    char windDirection[3];
    uint8_t windSpeed;
    uint8_t windRank;
    uint8_t humidity;
};

int parseWeather(char* s, Weather* weather);
void printWeather(Weather* w);

#endif