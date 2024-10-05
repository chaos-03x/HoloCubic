// WiFiManager.cpp
#include "WiFiManager.h"
#include <Arduino.h> // Arduino库

const char* ssid = "CMCC-awmy-2023";  // WiFi账号
const char* password = "20232323";     // WiFi密码

void ConnectWIFI(unsigned long timeout) {
    // 连接网络
    Serial.print("Connecting to WIFI");
    WiFi.begin(ssid, password);

    unsigned long startTime = millis(); // 记录开始时间

    // 等待WiFi连接
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        // 检查是否超时
        if (millis() - startTime >= timeout) {
            Serial.println("");
            Serial.println("Connection timed out."); // 超时提示
            // return; // 退出函数
            startTime = millis();
        }
    }

    Serial.println("WiFi connected"); // 连接成功
    Serial.print("IP address: ");      // 打印IP地址
    Serial.println(WiFi.localIP());
}
