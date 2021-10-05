#include "weather.h"


int parseWeather(char* s, Weather* weather){
    float n = 0;
    uint8_t i = 13;
    uint8_t p = 0;
    while (p < 10){
        if (p == 6){
            uint8_t k = 0;
            while (s[i + k] != ';'){
            *(weather->windDirection + k) = s[i+k];
                k++;
            }
            *(weather->windDirection + k + 1) = '\0';
            i += k + 1;
            p++;
            continue;
        }
        n = atof(&s[i]);
        switch(p){
            case 0:
            weather->dayCode = n;break;
            case 1:
            weather->nightCode = n;break;
            case 2:
            weather->highTemp = n;break;
            case 3:
            weather->lowTemp = n;break;
            case 4:
            weather->rainFall = n;break;
            case 5:
            weather->precip = n;break;
            case 7:
            weather->windSpeed = n;break;
            case 8:
            weather->windRank = n;break;
            case 9:
            weather->humidity = n;break;
        }
        while (s[i] != ';'){
            i ++;
        }
        i++;
        p++;
    }
    return i;
}