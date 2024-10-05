// WiFiManager.h
#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <WiFi.h> // WiFi库

// WiFi连接信息
extern const char* ssid;
extern const char* password;

// 连接WiFi的函数声明
void ConnectWIFI(unsigned long timeout);

#endif // WIFIMANAGER_H
