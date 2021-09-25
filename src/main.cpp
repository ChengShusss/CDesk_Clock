// #include <WiFi.h>
// #include <WebServer.h>
// #include <ESPmDNS.h>
// #include <esp_wifi.h>
// # include "main.h"

// const char* AP_SSID  = "ESP32_Config"; //热点名称
// String wifi_ssid = "";
// String wifi_pass = "";
// String scanNetworksID = "";//用于储存扫描到的WiFi

// #define ROOT_HTML  "<!DOCTYPE html><html><head><title>WIFI Config by lwang</title><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head><style type=\"text/css\">.input{display: block; margin-top: 10px;}.input span{width: 100px; float: left; float: left; height: 36px; line-height: 36px;}.input input{height: 30px;width: 200px;}.btn{width: 120px; height: 35px; background-color: #000000; border:0px; color:#ffffff; margin-top:15px; margin-left:100px;}</style><body><form method=\"GET\" action=\"connect\"><label class=\"input\"><span>WiFi SSID</span><input type=\"text\" name=\"ssid\"></label><label class=\"input\"><span>WiFi PASS</span><input type=\"text\"  name=\"pass\"></label><input class=\"btn\" type=\"submit\" name=\"submit\" value=\"Submie\"> <p><span> Nearby wifi:</P></form>"
// WebServer server(80);

// #define RESET_PIN   13  //用于删除WiFi信息

// void setup() {

//   Serial.begin(115200);
//   pinMode(RESET_PIN, INPUT_PULLUP);

//   // 连接WiFi
//   if (!AutoConfig())
//   {
//     wifi_Config();
//   }

//   //用于删除已存WiFi
//   if (digitalRead(RESET_PIN) == LOW) {
//     delay(1000);
//     esp_wifi_restore();
//     delay(10);
//     ESP.restart();  //复位esp32
//   }
// }

// void loop() {
//   server.handleClient();
//   while (WiFi.status() == WL_CONNECTED) {
//     //WIFI已连接

//   }
// }

// //用于配置WiFi
// void wifi_Config()
// {
//   Serial.println("scan start");
//   // 扫描附近WiFi
//   int n = WiFi.scanNetworks();
//   Serial.println("scan done");
//   if (n == 0) {
//     Serial.println("no networks found");
//     scanNetworksID = "no networks found";
//   } else {
//     Serial.print(n);
//     Serial.println(" networks found");
//     for (int i = 0; i < n; ++i) {
//       // Print SSID and RSSI for each network found
//       Serial.print(i + 1);
//       Serial.print(": ");
//       Serial.print(WiFi.SSID(i));
//       Serial.print(" (");
//       Serial.print(WiFi.RSSI(i));
//       Serial.print(")");
//       Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
//       scanNetworksID += "<P>" + WiFi.SSID(i) + "</P>";
//       delay(10);
//     }
//   }
//   Serial.println("");

//   WiFi.mode(WIFI_AP);//配置为AP模式
//   boolean result = WiFi.softAP(AP_SSID, ""); //开启WIFI热点
//   if (result)
//   {
//     IPAddress myIP = WiFi.softAPIP();
//     //打印相关信息
//     Serial.println("");
//     Serial.print("Soft-AP IP address = ");
//     Serial.println(myIP);
//     Serial.println(String("MAC address = ")  + WiFi.softAPmacAddress().c_str());
//     Serial.println("waiting ...");
//   } else {  //开启热点失败
//     Serial.println("WiFiAP Failed");
//     delay(3000);
//     ESP.restart();  //复位esp32
//   }

//   if (MDNS.begin("esp32")) {
//     Serial.println("MDNS responder started");
//   }

//   //首页
//   server.on("/", []() {
//     server.send(200, "text/html", ROOT_HTML + scanNetworksID + "</body></html>");
//   });

//   //连接
//   server.on("/connect", []() {

//     server.send(200, "text/html", "<html><body><font size=\"10\">successd,wifi connecting...<br />Please close this page manually.</font></body></html>");

//     WiFi.softAPdisconnect(true);
//     //获取输入的WIFI账户和密码
//     wifi_ssid = server.arg("ssid");
//     wifi_pass = server.arg("pass");
//     server.close();
//     WiFi.softAPdisconnect();
//     Serial.println("WiFi Connect SSID:" + wifi_ssid + "  PASS:" + wifi_pass);

//     //设置为STA模式并连接WIFI
//     WiFi.mode(WIFI_STA);
//     WiFi.begin(wifi_ssid.c_str(), wifi_pass.c_str());
//     uint8_t Connect_time = 0; //用于连接计时，如果长时间连接不成功，复位设备
//     while (WiFi.status() != WL_CONNECTED) {  //等待WIFI连接成功
//       delay(500);
//       Serial.print(".");
//       Connect_time ++;
//       if (Connect_time > 80) {  //长时间连接不上，复位设备
//         Serial.println("Connection timeout, check input is correct or try again later!");
//         delay(3000);
//         ESP.restart();
//       }
//     }
//     Serial.println("");
//     Serial.println("WIFI Config Success");
//     Serial.printf("SSID:%s", WiFi.SSID().c_str());
//     Serial.print("  LocalIP:");
//     Serial.print(WiFi.localIP());
//     Serial.println("");

//   });
//   server.begin();
// }

// //用于上电自动连接WiFi
// bool AutoConfig()
// {
//   WiFi.begin();
//   for (int i = 0; i < 20; i++)
//   {
//     int wstatus = WiFi.status();
//     if (wstatus == WL_CONNECTED)
//     {
//       Serial.println("WIFI SmartConfig Success");
//       Serial.printf("SSID:%s", WiFi.SSID().c_str());
//       Serial.printf(", PSW:%s\r\n", WiFi.psk().c_str());
//       Serial.print("LocalIP:");
//       Serial.print(WiFi.localIP());
//       Serial.print(" ,GateIP:");
//       Serial.println(WiFi.gatewayIP());
//       return true;
//     }
//     else
//     {
//       Serial.print("WIFI AutoConfig Waiting......");
//       Serial.println(wstatus);
//       delay(1000);
//     }
//   }
//   Serial.println("WIFI AutoConfig Faild!" );
//   return false;
// }


/** SetDateTime.cpp
 *
 * Example of setting the date and time to the RTC.
 *
 * @version 1.0.1
 * @author Rafa Couto <caligari@treboada.net>
 * @license GNU Affero General Public License v3.0
 * @see https://github.com/Treboada/Ds1302
 *
 */

#include <Arduino.h>
#include <Ds1302.h>

#define PIN_ENA 27
#define PIN_CLK 25
#define PIN_DAT 26

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

const static char* WeekDays[] =
{
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday",
    "Sunday"
};


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