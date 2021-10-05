#include <network.h>

Network::Network():
        server(80),
        wifi_ssid(""),
        wifi_pass(""),
        scanNetworksID("")
{
    AP_SSID  = "ESP32_Config"; //热点名称
}

void Network::setupWifi(char* ssid, char* pwd)
{
  delay(10);
  Serial.println("connecting WIFI");
  WiFi.begin(ssid, pwd);
}

void Network::wifiConfig(void){
    
    Serial.println("scan start");
    // 扫描附近WiFi
    int n = WiFi.scanNetworks();
    Serial.println("scan done");
    if (n == 0) {
        Serial.println("no networks found");
        scanNetworksID = "no networks found";
    } else {
        Serial.print(n);
        Serial.println(" networks found");
        for (int i = 0; i < n; ++i) {
        // Print SSID and RSSI for each network found
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print(WiFi.SSID(i));
        Serial.print(" (");
        Serial.print(WiFi.RSSI(i));
        Serial.print(")");
        Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
        scanNetworksID += "<P>" + WiFi.SSID(i) + "</P>";
        delay(10);
        }
    }
    Serial.println("");

    WiFi.mode(WIFI_AP);//配置为AP模式
    boolean result = WiFi.softAP(AP_SSID, ""); //开启WIFI热点
    if (result)
    {
        IPAddress myIP = WiFi.softAPIP();
        //打印相关信息
        Serial.println("");
        Serial.print("Soft-AP IP address = ");
        Serial.println(myIP);
        Serial.println(String("MAC address = ")  + WiFi.softAPmacAddress().c_str());
        Serial.println("waiting ...");
    } else {  //开启热点失败
        Serial.println("WiFiAP Failed");
        delay(3000);
        ESP.restart();  //复位esp32
    }

    if (MDNS.begin("esp32")) {
        Serial.println("MDNS responder started");
    }

    //首页
    server.on("/", [&]() {
        server.send(200, "text/html", ROOT_HTML + scanNetworksID + "</body></html>");
    });

    //连接
    server.on("/connect", [&]() {

        server.send(200, "text/html", "<html><body><font size=\"10\">successd,wifi connecting...<br />Please close this page manually.</font></body></html>");

        WiFi.softAPdisconnect(true);
        //获取输入的WIFI账户和密码
        wifi_ssid = server.arg("ssid");
        wifi_pass = server.arg("pass");
        server.close();
        WiFi.softAPdisconnect();
        Serial.println("WiFi Connect SSID:" + wifi_ssid + "  PASS:" + wifi_pass);

        //设置为STA模式并连接WIFI
        WiFi.mode(WIFI_STA);
        WiFi.begin(wifi_ssid.c_str(), wifi_pass.c_str());
        uint8_t Connect_time = 0; //用于连接计时，如果长时间连接不成功，复位设备
        while (WiFi.status() != WL_CONNECTED) {  //等待WIFI连接成功
        delay(500);
        Serial.print(".");
        Connect_time ++;
        if (Connect_time > 80) {  //长时间连接不上，复位设备
            Serial.println("Connection timeout, check input is correct or try again later!");
            delay(3000);
            ESP.restart();
        }
        }
        Serial.println("");
        Serial.println("WIFI Config Success");
        Serial.printf("SSID:%s", WiFi.SSID().c_str());
        Serial.print("  LocalIP:");
        Serial.print(WiFi.localIP());
        Serial.println("");

    });
    server.begin();
}

bool Network::autoConfig(void){
    int wstatus = WiFi.status();
    if (wstatus == WL_CONNECTED)
    {
        Serial.println("WIFI SmartConfig Success");
        Serial.printf("SSID:%s", WiFi.SSID().c_str());
        Serial.printf(", PSW:%s\r\n", WiFi.psk().c_str());
        Serial.print("LocalIP:");
        Serial.print(WiFi.localIP());
        Serial.print(" ,GateIP:");
        Serial.println(WiFi.gatewayIP());
        return true;
    }
    else
    {
        Serial.print("WIFI AutoConfig Waiting......");
        Serial.println(wstatus);
    }
    return false;
}


bool Network::isConnected(void){
    return WiFi.isConnected();
}


void Network::handleHttpRequest(void){
    server.handleClient();
}


bool Network::syncTime(Clock* clk){
  HTTPClient http; // 声明HTTPClient对象
  uint8_t dt[] = {0, 0, 0, 0, 0, 0, 0};
  uint8_t i = 0;
  uint8_t p = 0;
  bool flag = false;

  http.begin(TIME_HOST); // 准备启用连接
  Serial.println("Start to get info.");

  int httpCode = http.GET(); // 发起GET请求

  if (httpCode > 0) // 如果状态码大于0说明请求过程无异常
  {
    if (httpCode == HTTP_CODE_OK) // 请求被服务器正常响应，等同于httpCode == 200
    {
      String payload = http.getString(); // 读取服务器返回的响应正文数据
                                         // 如果正文数据很多该方法会占用很大的内存
      i = 10;
      while(p <= 6){
        while (payload[i] < '0' || payload[i] > '9') i++;
        while (payload[i] >= '0' && payload[i] <= '9'){
          dt[p] = (dt[p] * 10 + payload[i] - '0' ) % 100;
          i++;
        }
        p++;
      }
      clk->setTime(dt[0], dt[1], dt[2], dt[4], dt[5], dt[6], dt[3]);
      Serial.println("Setting Time Success...");
      flag = true;
    }
  }
  else
  {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end(); // 结束当前连接
  return flag;
}


bool Network::getWeather(Weather* today, Weather* tomorrow){
  HTTPClient http; // 声明HTTPClient对象
  bool flag = false;

  http.begin(WEATHER_HOST); // 准备启用连接
  Serial.println("Start to get Weather info.");

  int httpCode = http.GET(); // 发起GET请求

  if (httpCode > 0) // 如果状态码大于0说明请求过程无异常
  {
    if (httpCode == HTTP_CODE_OK) // 请求被服务器正常响应，等同于httpCode == 200
    {
      String payload = http.getString(); // 读取服务器返回的响应正文数据
                                         // 如果正文数据很多该方法会占用很大的内存
      Serial.println(payload);
      if (payload[0] == 'T'){
          uint8_t offset = parseWeather(&(payload[2]), today);
          parseWeather(&(payload[offset + 2]), tomorrow);
      }
      else{
          Serial.println("Return No Weather Date");
      }
    }
  }
  else
  {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end(); // 结束当前连接
  return flag;
}