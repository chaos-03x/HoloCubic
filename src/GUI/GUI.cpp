#include "GUI.h"
#include "WeatherAPI.h"
#include "PicMap.h"
#include "WIFIManager.h"
#include "FontManager.h"
#include <TFT_eSPI.h>

#define DEFAULT_FONT &FreeSans9pt7b  // 字体
#define DEFAULT_TEXT_COLOR TFT_WHITE  // 字体颜色
#define DEFAULT_BG_COLOR TFT_BLACK    // 背景色
#define DEFAULT_FONT_SIZE 1           // 字体大小

#define SCREEN_OFFSET 7       // 边界无法显示的区域
#define SCREEN_WIDTH 128      // 屏幕宽度
#define SCREEN_HEIGHT 128     // 屏幕高度
#define DISPLAY_WIDTH (SCREEN_WIDTH - 2 * SCREEN_OFFSET)  // 有效显示区域宽度
#define DISPLAY_HEIGHT (SCREEN_HEIGHT - 2 * SCREEN_OFFSET) // 有效显示区域高度

// 统一文本显示函数
void showUnifiedText(TFT_eSPI* tft, int x, int y, const char* text) {
    tft->setTextColor(DEFAULT_TEXT_COLOR, DEFAULT_BG_COLOR);  // 设置文字颜色和背景
    // tft->setFreeFont(DEFAULT_FONT);  // 使用统一字体
    tft->setTextSize(DEFAULT_FONT_SIZE);  // 统一文字大小
    tft->setCursor(x + SCREEN_OFFSET, y + SCREEN_OFFSET);  // 加入偏移量，避免文字超出边界
    tft->print(text);  // 显示文字
}

GUI::GUI(TFT_eSPI* tft, WeatherAPI& weatherAPI)
    : _tft(tft), _weatherAPI(weatherAPI), _currentPage(0), _xValue(0), _yValue(0), _zValue(0) {}

void GUI::init() {
    // 初始化TFT屏幕
    _tft->init();
    // _tft->setRotation(4); // 分光棱镜用
    _tft->setRotation(0); // 正常显示
    Serial.println("GUI_init:Rotation:4");
    _tft->fillScreen(DEFAULT_BG_COLOR);

    // 初始化页面
    _pages = {
        { PageType::PARENT }, // PAGE1
        { PageType::PARENT }, // PAGE2
        { PageType::PARENT }, // PAGE3
        { PageType::PARENT }, // PAGE4
        { PageType::PARENT }  // PAGE5
    };
    drawPage0();  // 显示连接WiFi的页面
}

void GUI::draw() {
    drawParentPage(_currentPage);
}

void GUI::ConnectingGUI() {
    ConnectWIFI(); // 连接WIFI
    if(isWIFIConnected()){
        _tft->fillScreen(TFT_WHITE);
        int flag = 0;
        for (int i = 0; i < 6; i++) {
            _tft->setSwapBytes(true);
            _tft->pushImage(14, 45, 100, 20, Loading[i]);
            delay(100); 
            if (i==5){i=0;}
            if (flag == 24){break;}
            flag++;
        }
        _tft->pushImage(14, 45, 100, 20, Loading[5]);
        if (_weatherAPI.fetchWeatherData()) {
            Serial.println("Weather data fetched successfully.");
            draw(); // 确保在获取数据后调用 draw()
        } else {
            Serial.println("Failed to fetch weather data.");
        }
    } else {
        for(int i=10; i>0; i--){
            _tft->fillScreen(TFT_BLACK);
            Serial.print("Connect failed...");
            showUnifiedText(_tft, 10, 20, "Connect failed...");
            showUnifiedText(_tft, 10, 35, "Please connect to");
            showUnifiedText(_tft, 10, 50, "WiFi:ESP32 1234567   8");
            showUnifiedText(_tft, 10, 68, "And access the fol  lowing IP address:");
            showUnifiedText(_tft, 10, 90, WiFi.softAPIP().toString().c_str());
            String countdown = String(i); // 将整数 i 转换为字符串
           showUnifiedText(_tft, 62, 105, countdown.c_str()); // 显示倒计时

            delay(1000); // 1秒
        }
        draw();
    }
}

void GUI::drawPage0() {
    _tft->fillScreen(DEFAULT_BG_COLOR);  // 统一背景色
    showUnifiedText(_tft, 10, 30, "GUI Starting...");
    ConnectingGUI();
}

void GUI::drawParentPage(int index) {
    switch (index) {
        case 0:
            drawPage1();
            break;
        case 1:
            drawPage2();
            break;
        case 2:
            drawPage3();
            break;
        case 3:
            drawPage4();
            break;
        case 4:
            drawPage5();
            break;
    }
}

void GUI::updateJoystick(int x, int y, int z) {
    _xValue = x;
    _yValue = y;
    _zValue = z;
}

void GUI::handleInput() {
    unsigned long currentTime = millis(); // 获取当前时间

    if (_xValue == 4095) { // 向左
        if (_currentPage > 0) {
            if (currentTime - lastPageChangeTime >= pageChangeDelay) {
                _currentPage--; // 向前翻父页面
                lastPageChangeTime = currentTime; // 更新翻页时间
                draw(); // 绘制当前页面
            }
        }
    } else if (_xValue == 0) { // 向右
        if (_currentPage < _pages.size() - 1) {
            if (currentTime - lastPageChangeTime >= pageChangeDelay) {
                _currentPage++; // 向后翻父页面
                lastPageChangeTime = currentTime; // 更新翻页时间
                draw(); // 绘制当前页面
            }
        }
    }

    if (_yValue == 0 && _currentPage == 2) {
        _currentMode = (_currentMode + 1) % 4; // 循环切换3种模式
        spectrumAnalyzer->setMode(_currentMode); // 设置新的频谱模式
        Serial.print("Mode changed:");
        Serial.println(_currentMode);
    }
}

void GUI::update() {
    draw();  // 重新绘制当前页面
}

/* 天气页 */
void GUI::drawPage1() {
    _tft->fillScreen(DEFAULT_BG_COLOR);
    _tft->setSwapBytes(true);

    String now_address = _weatherAPI.getAddress();
    String now_wea = _weatherAPI.getWeather();
    String now_high_tem = _weatherAPI.getHighTemperature();
    String now_low_tem = _weatherAPI.getLowTemperature();
    String now_rainfall = _weatherAPI.getRainfall();
    String now_wind_direction = _weatherAPI.getWindDirection();
    String now_hum = _weatherAPI.getHumidity();
    int weatherCode = _weatherAPI.getWeatherCode();

    if(isWIFIConnected()){
        _tft->pushImage(20, 0, 64, 64, weather[weatherCode], 0x0000);

        showMyFonts(_tft, 90, 20, now_address.c_str(), TFT_WHITE); // 地址
        showMyFonts(_tft, 90, 40, now_wea.c_str(), TFT_WHITE); // 天气

        _tft->pushImage(0, 65, 30, 30, temIcon); 
        _tft->pushImage(0, 95, 30, 30, humIcon);
        _tft->pushImage(55, 65, 30, 30, rainIcon);
        // _tft->pushImage(55, 95, 30, 30, windIcon); // 风 优化

        showtext(_tft, 30, 75, 1, 1, TFT_WHITE, TFT_BLACK, now_high_tem + "/" + now_low_tem);
        showtext(_tft, 85, 75, 1, 1, TFT_WHITE, TFT_BLACK, now_rainfall + "mm");
        showtext(_tft, 30, 105, 1, 1, TFT_WHITE, TFT_BLACK, now_hum + "%");
        // String now_wind = now_wind_direction + "风";
        // showMyFonts(_tft, 85, 100, now_wind.c_str(), TFT_WHITE);
    }else{
        _tft->pushImage(14, 14, 100, 100, offline, 0xF81F);
        Serial.println("Page1:push image");
    }

}

void GUI::drawPage2() {
    _tft->setSwapBytes(true);
    if (spectrumAnalyzer == nullptr) {
        spectrumAnalyzer = new MusicSpectrum(_tft);
        spectrumAnalyzer->begin();
    }
    spectrumAnalyzer->update();
}

void GUI::drawPage3() {
    _tft->fillScreen(DEFAULT_BG_COLOR);
    showUnifiedText(_tft, 50, 50, "Welcome to Page 3");
}

void GUI::drawPage4() {
    _tft->fillScreen(DEFAULT_BG_COLOR);
    showUnifiedText(_tft, 50, 50, "Welcome to Page 4");
}

void GUI::drawPage5() {
    _tft->fillScreen(DEFAULT_BG_COLOR);
    showUnifiedText(_tft, 50, 50, "Welcome to Page 5");
}
