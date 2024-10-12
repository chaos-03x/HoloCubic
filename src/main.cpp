#include <TFT_eSPI.h>
#include <WebServer.h>
#include "WeatherAPI.h"
#include "GUI.h"
#include "WIFIManager.h"

// 记录上次检查WiFi的时间
unsigned long previousMillis = 0;  
const long interval = 1800000; // 30分钟，单位是毫秒
// 定义摇杆引脚
const int xPin = 34; // X轴
const int yPin = 35; // Y轴
const int zPin = 32; // Z轴
const char* apiKey = "SFIdN0wDbUHnhNI3i";
// const char* apiKey = "SkLq7cp0BN7VAE0Yd";
extern WebServer server; 
// 实例化tft_eSPI
TFT_eSPI tft = TFT_eSPI();
// 实例化WeaherAPI
WeatherAPI weatherAPI("api.seniverse.com", apiKey);
GUI gui(&tft, weatherAPI); 


void setup() {
    Serial.begin(115200); // 初始化串口
    Serial.println("Initializing...");
    gui.init(); // 初始化GUI
    pinMode(zPin,INPUT); // Z轴坏的，封印
    startWifiAP(); // 启动Wifi热点
    startHTTPServer(); // 启动 HTTP 服务器

    Serial.println("Setup done");
}

void loop() {
    // 读取X和Y的模拟值
    int xValue = analogRead(xPin);
    int yValue = analogRead(yPin);
    int zValue = 0;
    // int zValue = digitalRead(zPin);
    // Serial.print("x:");
    // Serial.print(xValue);
    // Serial.print("|");
    // Serial.print("y:");
    // Serial.print(yValue);
    // Serial.println();



    // 监听http server
    server.handleClient(); 

    // 获取当前时间
    unsigned long currentMillis = millis();

    // 每半个小时检测一次WiFi连接状态，如果断开进入连接界面
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis; // 每30分钟
        if (!isWIFIConnected()) {
            gui.drawPage0(); // 进入Page0
        }
    }

    
    // 持续刷新频谱图（当处于Page2时）
    if (gui.getCurrentPage() == 1) { 
        if (gui.spectrumAnalyzer != nullptr) {
            gui.spectrumAnalyzer->update();  // 持续更新频谱
            delay(5); // 控制刷新频率
        }
    }

    // 将值传递给GUI
    gui.updateJoystick(xValue, yValue, zValue);
    gui.handleInput(); // 处理输入
    delay(50); // 延迟以便处理时间
}
