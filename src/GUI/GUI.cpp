#include "GUI.h"
#include "WeatherAPI.h"
#include "PicMap.h"
#include "WIFIManager.h"
#include "FontManager.h"
#include <TFT_eSPI.h>

GUI::GUI(TFT_eSPI* tft, WeatherAPI& weatherAPI) 
    : _tft(tft), _weatherAPI(weatherAPI), _currentPage(0), _xValue(0), _yValue(0), _zValue(0) {}

void GUI::init() {
    // 初始化TFT屏幕
    _tft->init();
    _tft->setRotation(1);
    _tft->fillScreen(TFT_BLACK);

    // 初始化页面
    _pages = {
        { PageType::PARENT, { { PageType::CHILD }, { PageType::CHILD }, { PageType::CHILD }, { PageType::CHILD }, { PageType::CHILD } } }, // PAGE1
        { PageType::PARENT, { { PageType::CHILD }, { PageType::CHILD }, { PageType::CHILD }, { PageType::CHILD }, { PageType::CHILD } } }, // PAGE2
        { PageType::PARENT, { { PageType::CHILD }, { PageType::CHILD }, { PageType::CHILD }, { PageType::CHILD }, { PageType::CHILD } } }, // PAGE3
        { PageType::PARENT, { { PageType::CHILD }, { PageType::CHILD }, { PageType::CHILD }, { PageType::CHILD }, { PageType::CHILD } } }, // PAGE4
        { PageType::PARENT, { { PageType::CHILD }, { PageType::CHILD }, { PageType::CHILD }, { PageType::CHILD }, { PageType::CHILD } } }, // PAGE5
    };
    drawPage0();  // 显示连接WiFi的页面
}

void GUI::draw() {
    if (_inSubPage) {
        drawChildPage(_currentSubPage);
    } else {
        drawParentPage(_currentPage);
    }
}

void GUI::ConnectingGUI() {
    ConnectWIFI(); // 连接WIFI
    if(isWIFIConnected()){
        Serial.print("Connect Succeed!");
        _tft->print("Connect Succeed!");
        int flag = 0;
        for (int i = 0; i < 6; i++) {
            _tft->setSwapBytes(true);
            _tft->pushImage(14, 65, 100, 20, Loading[i]);
            delay(100); 
            if (i==5){i=0;}
            if (flag == 30){break;}
            flag++;
        }
        _tft->pushImage(14, 65, 100, 20, Loading[5]);
        if (_weatherAPI.fetchWeatherData()) {
            Serial.println("Weather data fetched successfully.");
            draw(); // 确保在获取数据后调用 draw()
        } else {
            Serial.println("Failed to fetch weather data.");
        }
    }else{
        Serial.print("Connect failed...");
        _tft->print("Connect failed...");
        startWifiAP();
        // 进入另一个循环等待连接
        while (true) {
            if (isWIFIConnected()) {
                closeWifiAP();
                draw();
                return; // 结束函数
            }
            delay(100); // 加入延迟以避免过于频繁的轮询
        }
    }
}

void GUI::drawPage0() {
    _tft->fillScreen(TFT_BLACK);
    _tft->setCursor(30, 60);
    _tft->print("GUI Starting......");
    // 进入WIFI连接界面
    ConnectingGUI();
}


void GUI::drawParentPage(int index) {
    // 根据 index 绘制父页面
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

void GUI::drawChildPage(int index) {
    // 根据子页面索引绘制子页面
    _tft->fillScreen(TFT_BLACK);
    _tft->setCursor(10, 10);
    _tft->print("Welcome to SubPage of Parent Page ");
    _tft->print(_currentPage + 1); // 显示当前父页面索引

   // 根据子页面索引执行不同的功能
    switch (_currentPage) {
        case 0:
            drawPage1_1();
            break;
        case 1:
            drawPage2_1();
            break;
        case 2:
            drawPage3_1();
            break;
        case 3:
            drawPage4_1();
            break;
        case 4:
            drawPage5_1();
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

    // 检测Z轴点击事件
    if (_zValue == 1) {
        // 进入或返回父页面
        if (_inSubPage) {
            // 返回父页面
            _inSubPage = false;
            _currentSubPage = 0; // 重置子页面索引
            draw(); // 绘制当前页面
        } else {
            // 进入子页面
            if (!_pages[_currentPage].subPages.empty()) {
                _inSubPage = true;
                _currentSubPage = 0; // 默认进入第一个子页面
                draw(); // 绘制当前页面
            }
        }
    }

    // 检测X轴的值
    if (_xValue == 0) { // 向左
        if (_inSubPage) {
            if (_currentSubPage > 0) {
                if (currentTime - lastPageChangeTime >= pageChangeDelay) {
                    _currentSubPage--; // 向前翻子页面
                    lastPageChangeTime = currentTime; // 更新翻页时间
                    draw(); // 绘制当前页面
                }
            }
        } else {
            if (_currentPage > 0) {
                if (currentTime - lastPageChangeTime >= pageChangeDelay) {
                    _currentPage--; // 向前翻父页面
                    lastPageChangeTime = currentTime; // 更新翻页时间
                    draw(); // 绘制当前页面
                }
            }
        }
    } else if (_xValue == 4095) { // 向右
        if (_inSubPage) {
            if (_currentSubPage < _pages[_currentPage].subPages.size() - 1) {
                if (currentTime - lastPageChangeTime >= pageChangeDelay) {
                    _currentSubPage++; // 向后翻子页面
                    lastPageChangeTime = currentTime; // 更新翻页时间
                    draw(); // 绘制当前页面
                }
            }
        } else {
            if (_currentPage < _pages.size() - 1) {
                if (currentTime - lastPageChangeTime >= pageChangeDelay) {
                    _currentPage++; // 向后翻父页面
                    lastPageChangeTime = currentTime; // 更新翻页时间
                    draw(); // 绘制当前页面
                }
            }
        }
    }

    // 检测Y轴的值（如果需要处理Y轴翻页逻辑，可以在这里添加）
    if (_yValue == 0) { // 向上
        // 处理向上的逻辑（如果需要）
    } else if (_yValue == 4095) { // 向下
        // 处理向下的逻辑（如果需要）
    }
}

void GUI::update() {
    // 如果有需要更新的内容，可以在这里实现
    draw();  // 重新绘制当前页面
}

/* 天气界面 */
void GUI::drawPage1() {
    _tft->fillScreen(TFT_BLACK);
    _tft->setSwapBytes(true);

        // 获取天气数据
    String now_address = _weatherAPI.getAddress();
    String now_wea = _weatherAPI.getWeather();
    String now_high_tem = _weatherAPI.getHighTemperature();
    String now_low_tem = _weatherAPI.getLowTemperature();
    String now_rainfall = _weatherAPI.getRainfall();
    String now_wind_direction = _weatherAPI.getWindDirection();
    String now_hum = _weatherAPI.getHumidity();
    int weatherCode = _weatherAPI.getWeatherCode();

    _tft->pushImage(20, 0, 64, 64, weather[weatherCode], 0x0000);
    Serial.println();
    Serial.print("WeatherCode:");
    Serial.println(weatherCode);
    Serial.print("now_address:");
    Serial.println(now_address);
    Serial.print("now_wea:");
    Serial.println(now_wea);
    Serial.print("now_high_tem:");
    Serial.println(now_high_tem);
    Serial.print("now_low_tem:");
    Serial.println(now_low_tem);
    Serial.print("now_rainfall:");
    Serial.println(now_rainfall);
    Serial.print("now_wind_direction:");
    Serial.println(now_wind_direction);
    Serial.print("now_hum:");
    Serial.println(now_hum);

    showMyFonts(_tft, 90, 20, now_address.c_str(), TFT_WHITE);
    showMyFonts(_tft,90, 40, now_wea.c_str(), TFT_WHITE);

    _tft->pushImage(0, 65, 30, 30, temIcon);
    _tft->pushImage(0, 95, 30, 30, humIcon);
    _tft->pushImage(55, 65, 30, 30, rainIcon);
    _tft->pushImage(55, 95, 30, 30, windIcon);

    showtext(_tft,30,75,1,1,TFT_WHITE,TFT_BLACK,now_high_tem + "/" + now_low_tem);
    showtext(_tft,85,75,1,1,TFT_WHITE,TFT_BLACK,now_rainfall +"mm");
    showtext(_tft,30,105,1,1,TFT_WHITE,TFT_BLACK,now_hum+"%");
    String now_wind = now_wind_direction + "风";
    showMyFonts(_tft,85, 100, now_wind.c_str(), TFT_WHITE);

    

}

/* 音乐频谱 */
void GUI::drawPage2() {
    _tft->fillScreen(TFT_WHITE);
    _tft->setCursor(1, 1);
    _tft->print("Welcome to Page 2");
    _tft->setSwapBytes(true);
    // _tft->pushImage(39, 39, 50, 50, rainfall_50_outline,0xF81F);
    // 绘制第二页的其他内容
}

/* 闹钟 */
void GUI::drawPage3() {
    _tft->fillScreen(TFT_WHITE);
    _tft->setCursor(1, 1);
    _tft->print("Welcome to Page 3");
    _tft->setSwapBytes(true);
    // _tft->pushImage(14, 14, 100, 100, rainfall_100_filled,0xF81F);
    // 绘制第三页的其他内容
}

/* AI大模型 */
void GUI::drawPage4() {
    _tft->fillScreen(TFT_WHITE);
    _tft->setCursor(1, 1);
    _tft->print("Welcome to Page 4");
    _tft->setSwapBytes(true);
    // _tft->pushImage(14, 14, 100, 190, rainfall_100_outline,0xF81F);
    // 绘制第四页的其他内容
}

/* 设置 */
void GUI::drawPage5() {
    _tft->fillScreen(TFT_WHITE);
    _tft->setCursor(1, 1);
    _tft->print("Welcome to Page 5");
    _tft->setSwapBytes(true);
    // _tft->pushImage(14, 14, 100, 190, rainfall_100_outline,0xF81F);
    // 绘制第五页的其他内容
}

void GUI::drawPage1_1(){

}
void GUI::drawPage2_1(){

}
void GUI::drawPage3_1(){

}
void GUI::drawPage4_1(){

}
void GUI::drawPage5_1(){

}

