// WiFiManager.h
#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <Arduino.h> // Arduino库
#include <WiFi.h> // WiFi库
#include <WebServer.h>
#include <Update.h> 

// WiFi账号密码
extern const char* ssid;
extern const char* password;

// 连接WiFi的函数声明
bool ConnectWIFI();
bool ConnectWIFI(const char* ssid, const char* password);

// 判断WIFI连接状态
bool isWIFIConnected();

// 开启HTTP服务器
void startHTTPServer();

// 开启WIFI热点
void startWifiAP();
// 关闭WIFI热点
void closeWifiAP();

// NTP对时
void getTime();

#endif // WIFIMANAGER_H
