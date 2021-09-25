#include <clock.h>

#include <Ds1302.h>

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
Ds1302 rtc(PIN_ENA, PIN_CLK, PIN_DAT);


uint8_t parseDigits(char* str, uint8_t count)
{
    uint8_t val = 0;
    while(count-- > 0) val = (val * 10) + (*str++ - '0');
    return val;
}


// void setup()
// {
//     // initialize the RTC
//     rtc.init();

//     delay(3000);
//     Serial.begin(115200);
//     Serial.println("Input the date and time (YYMMDDWhhmmss): ");
// }


// void loop()
// {
//     static char buffer[13];
//     static uint8_t char_idx = 0;

//     if (char_idx == 13)
//     {
//         // structure to manage date-time
//         Ds1302::DateTime dt;

//         dt.year = parseDigits(buffer, 2);
//         dt.month = parseDigits(buffer + 2, 2);
//         dt.day = parseDigits(buffer + 4, 2);
//         dt.dow = parseDigits(buffer + 6, 1);
//         dt.hour = parseDigits(buffer + 7, 2);
//         dt.minute = parseDigits(buffer + 9, 2);
//         dt.second = parseDigits(buffer + 11, 2);

//         // set the date and time
//         rtc.setDateTime(&dt);
//         Serial.println("Set time success.");

//         char_idx = 0;
//     }

//     if (Serial.available())
//     {
//         buffer[char_idx++] = Serial.read();
//     }
// }




void setup()
{
    Serial.begin(115200);

    // initialize the RTC
    rtc.init();

    // test if clock is halted and set a date-time (see example 2) to start it
    if (rtc.isHalted())
    {
        Serial.println("RTC is halted. Setting time...");

        Ds1302::DateTime dt = {
            .year = 17,
            .month = Ds1302::MONTH_OCT,
            .day = 3,
            .hour = 4,
            .minute = 51,
            .second = 30,
            .dow = Ds1302::DOW_TUE
        };

        rtc.setDateTime(&dt);
    }
}


void loop()
{
    // get the current time
    Ds1302::DateTime now;
    rtc.getDateTime(&now);

    static uint8_t last_second = 0;
    if (last_second != now.second)
    {
        last_second = now.second;

        Serial.print("20");
        Serial.print(now.year);    // 00-99
        Serial.print('-');
        if (now.month < 10) Serial.print('0');
        Serial.print(now.month);   // 01-12
        Serial.print('-');
        if (now.day < 10) Serial.print('0');
        Serial.print(now.day);     // 01-31
        Serial.print(' ');
        Serial.print(WeekDays[now.dow - 1]); // 1-7
        Serial.print(' ');
        if (now.hour < 10) Serial.print('0');
        Serial.print(now.hour);    // 00-23
        Serial.print(':');
        if (now.minute < 10) Serial.print('0');
        Serial.print(now.minute);  // 00-59
        Serial.print(':');
        if (now.second < 10) Serial.print('0');
        Serial.print(now.second);  // 00-59
        Serial.println();
    }

    delay(100);
}