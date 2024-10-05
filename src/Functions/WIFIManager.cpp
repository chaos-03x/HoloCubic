// WiFiManager.cpp
#include <Arduino.h> // Arduino库
#include <WiFi.h>
#include <WebServer.h>
#include "WiFiManager.h"


const char* ssid = "CMCC-awmy-2023";  // WiFi账号
const char* password = "20232323";     // WiFi密码


// 判断WIFI是否连接
bool isWIFIConnected(){
    if(WiFi.status() != WL_CONNECTED){
        return false;
    }else{
        return true;
    }
}

// 开启WIFI热点
void startWifiAP() {
    const char *ssid = "ESP32";
    const char *password = "12345678";

    // 配置WiFi热点
    WiFi.softAP(ssid, password);
    Serial.println("WiFi AP started");
    Serial.print("IP address: ");
    Serial.println(WiFi.softAPIP());
}


WebServer server(80); // 创建HTTP服务器，端口80

// 显示HTML页面，选择WiFi并连接
void handleRoot() {
    String html = "<html><body>";
    html += "<h1>WiFi Configuration</h1>";
    html += "<form action='/connect' method='POST'>";
    html += "WiFi Name: <input type='text' name='ssid'><br>";
    html += "Password: <input type='password' name='password'><br>";
    html += "<input type='submit' value='Connect'>";
    html += "</form>";
    html += "<form action='/restart' method='POST'>";
    html += "<input type='submit' value='Restart ESP32'>";
    html += "</form>";
    html += "</body></html>";
    
    server.send(200, "text/html", html);
}

// 处理WiFi连接请求
void handleConnect() {
    String ssid = server.arg("ssid");
    String password = server.arg("password");
    
    server.send(200, "text/html", "<html><body><h1>Connecting to WiFi...</h1></body></html>");
    ConnectWIFI(ssid.c_str(), password.c_str()); // 使用用户输入的WiFi名称和密码连接
}

// 重启ESP32
void handleRestart() {
    server.send(200, "text/html", "<html><body><h1>Restarting...</h1></body></html>");
    ESP.restart();
}

// 开启HTTP服务器
void startHTTPServer() {
    server.on("/", handleRoot);        // 主页路由
    server.on("/connect", handleConnect);  // 连接WiFi的路由
    server.on("/restart", handleRestart);  // 重启ESP32的路由
    
    server.begin();  // 启动HTTP服务器
    Serial.println("HTTP server started");
}

// 连接WIFI
void ConnectWIFI() {
    WiFi.begin(ssid, password);
    
    // 等待WiFi连接
    int timeout = 15000;  // 设定超时
    while (WiFi.status() != WL_CONNECTED && timeout > 0) {
        delay(500);
        Serial.print(".");
        timeout -= 500;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Connected to WiFi");
    } else {
        Serial.println("Failed to connect to WiFi");
    }
}

// 连接WIFI
void ConnectWIFI(const char* ssid, const char* password) {
    WiFi.begin(ssid, password);
    
    // 等待WiFi连接
    int timeout = 15000;  // 设定超时
    while (WiFi.status() != WL_CONNECTED && timeout > 0) {
        delay(500);
        Serial.print(".");
        timeout -= 500;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Connected to WiFi");
    } else {
        Serial.println("Failed to connect to WiFi");
    }
}

