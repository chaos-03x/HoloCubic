#include <SPI.h>
#include <TFT_eSPI.h>
#include "GUI.h"
#include "WIFIManager.h"

TFT_eSPI tft = TFT_eSPI();
GUI gui(&tft);

// 记录上次检查WiFi的时间
unsigned long previousMillis = 0; 
const long interval = 1800000; // 30分钟，单位是毫秒

// 定义摇杆引脚
const int xPin = 34; // X轴
const int yPin = 35; // Y轴
const int zPin = 32; // Z轴

void setup() {
    Serial.begin(115200); // 初始化串口
    Serial.println("Initializing...");

    gui.init(); // 初始化GUI
    pinMode(zPin,INPUT);
    
    
    Serial.println("Setup done");
}

void loop() {
    // 读取X和Y的模拟值
    int xValue = analogRead(xPin);
    int yValue = analogRead(yPin);
    int zValue = digitalRead(zPin);

    // 打印值到串口监视器（可选）
    // Serial.print("X: ");
    // Serial.print(xValue);
    // Serial.print(" | Y: ");
    // Serial.print(yValue);
    // Serial.print(" | Z: ");
    // Serial.println(zValue);

    // 获取当前时间
    unsigned long currentMillis = millis();

    // 检查是否需要进行WiFi状态检测
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis; // 每30分钟，判断一次WIFI状态

        // 判断WiFi是否已连接
        if (!isWIFIConnected()) {
            gui.drawPage0(); // 未连接，进入Page0
        }
    }

    // 将值传递给GUI
    gui.updateJoystick(xValue, yValue, zValue);
    gui.handleInput(); // 处理输入
    delay(50); // 延迟以便处理时间
}
