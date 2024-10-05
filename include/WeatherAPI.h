// WeatherAPI.h
#ifndef WEATHEAPI_H
#define WEATHEAPI_H

#include <ArduinoJson.h>  // Json库
#include <WiFiClient.h>   // WiFiClient库

// 获取天气数据的函数声明
void getWeatherData(String &address, String &weather, String &temperature);

#endif // WEATHERAPI_H
