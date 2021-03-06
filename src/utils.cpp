#include "utils.h"


void copyDateTime(Ds1302::DateTime* src, Ds1302::DateTime* dst){
    dst->year = src->year;
    dst->month = src->month;
    dst->day = src->day;
    dst->hour = src->hour;
    dst->minute = src->minute;
    dst->second = src->second;
    dst->dow = src->dow;
}


void castTimeToString(Ds1302::DateTime* time, char* dst){
    dst[0] = time->hour / 10 + '0';
    dst[1] = time->hour % 10 + '0';
    dst[2] = ':';
    dst[3] = time->minute / 10 + '0';
    dst[4] = time->minute % 10 + '0';
    dst[5] = '\0';
}

void castDateToString(Ds1302::DateTime* dt, char* dst){
    dst[0] = '2';
    dst[1] = '0';
    dst[2] = dt->year / 10 + '0';
    dst[3] = dt->year % 10 + '0';
    dst[4] = '-';
    dst[5] = dt->month / 10 + '0';
    dst[6] = dt->month % 10 + '0';
    dst[7] = '-';
    dst[8] = dt->day / 10 + '0';
    dst[9] = dt->day % 10 + '0';
    dst[10] = '\0';
}



uint16_t binarySerch(const uint16_t* arr, uint16_t size, uint16_t target){
    uint16_t l, r, mid;
    l = 0;
    r = size;
    while (l <= r){
        mid = l + ((r-l) >> 1);
        if (*(arr + mid) == target){
            return mid;
        }
        if (*(arr + mid) > target){
            r = mid - 1;
        } else {
            l = mid + 1;
        }
    }
    return 65535;
}