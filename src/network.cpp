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

void Network::handleClient(void){
    server.handleClient();
}

void Network::setUpHttpClient(char* host)
{
  req = (String)host;
  Serial.println(req);
  if (http_client.begin(req))
  {
    Serial.println("HTTPclient setUp done!");
  }
}

bool Network::isConnected(void){
    return WiFi.isConnected();
}


void Network::handleHttpRequest(void){
    server.handleClient();
}