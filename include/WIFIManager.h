// WiFiManager.h
#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <WiFi.h> // WiFi库

// WiFi账号密码
extern const char* ssid;
extern const char* password;

// 连接WiFi的函数声明
void ConnectWIFI();
void ConnectWIFI(const char* ssid, const char* password);

// 判断WIFI连接状态
bool isWIFIConnected();

// 开启HTTP服务器
void startHTTPServer();

#endif // WIFIMANAGER_H
