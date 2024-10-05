// WeatherManager.cpp
#include <Arduino.h> // Arduino库
#include <WiFi.h>
#include "WeatherAPI.h"
#include "WIFIManager.h"

const char* host = "api.seniverse.com";  // 心知天气服务器地址

void getWeatherData(String &address, String &weather, String &temperature) {
    // 判断WIFI是否连接
    if (WiFi.status() != WL_CONNECTED)
    {
        ConnectWIFI(10000);
    }
    

    // 创建TCP连接
    WiFiClient client;
    const int httpPort = 80;

    // 尝试连接到指定的主机
    Serial.println("Connecting....");
    if (!client.connect(host, httpPort)) {
        Serial.println("Connection failed");  // 网络请求无响应，打印连接失败
        return;
    }

    // URL请求地址
    String url = "/v3/weather/now.json?key=SFIdN0wDbUHnhNI3i&location=fujian%20fuzhou&language=zh-Hans&unit=c";

    // 发送网络请求
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");

    // 等待请求响应
    delay(5000);

    // 定义answer变量用来存放请求网络服务器后返回的数据
    String answer;
    while (client.available()) {
        String line = client.readStringUntil('\r');
        answer += line;  // 将每行数据拼接到answer中
    }

    // 断开服务器连接
    client.stop();
    Serial.println();
    Serial.println("Closing connection");

    // 获得JSON格式的数据
    String jsonAnswer;
    int jsonIndex;

    // 找到有用的返回数据位置，返回头不要
    for (int i = 0; i < answer.length(); i++) {
        if (answer[i] == '{') {
            jsonIndex = i;
            break;
        }
    }
    jsonAnswer = answer.substring(jsonIndex);  // 提取JSON数据
    Serial.println();
    Serial.println("JSON answer: ");
    Serial.println(jsonAnswer);

    // JSON数据处理
    DynamicJsonDocument doc(1024);  // 创建动态JSON文档
    DeserializationError error = deserializeJson(doc, jsonAnswer);

    // 检查JSON解析是否成功
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;  // 解析失败，退出函数
    }

    // 提取所需的天气数据
    JsonObject results_0 = doc["results"][0];
    JsonObject results_0_location = results_0["location"];
    JsonObject results_0_now = results_0["now"];

    address = results_0_location["name"].as<String>();          // 地址
    weather = results_0_now["text"].as<String>();               // 天气描述
    temperature = results_0_now["temperature"].as<String>();     // 温度

    // 打印结果
    Serial.println("Address:");
    Serial.println(address);
    Serial.println("Weather:");
    Serial.println(weather);
    Serial.println("Temperature:");
    Serial.println(temperature);
}
