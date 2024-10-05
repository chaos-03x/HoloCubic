#include "GUI.h"
#include "WeatherAPI.h"
#include "PicMap.h"

GUI::GUI(TFT_eSPI* tft) : _tft(tft), _currentPage(0), _xValue(0), _yValue(0), _zValue(0) {}

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

    draw();
}

void GUI::draw() {
    if (_inSubPage) {
        drawChildPage(_currentSubPage);
    } else {
        drawParentPage(_currentPage);
    }
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

void GUI::drawPage1() {
    _tft->fillScreen(TFT_WHITE);
    _tft->setCursor(1, 1);
    _tft->print("Welcome to Page 1");
    _tft->setSwapBytes(true);
    // _tft->pushImage(39, 39, 50, 50, rainfall_50_filled);
    drawImage(rainfall_50_filled, 50, 50, 0, 0);

    // 绘制第一页的其他内容
    // String address;
    // String weather;
    // String temperature;
    // getWeatherData(address, weather, temperature);
}

void GUI::drawPage2() {
    _tft->fillScreen(TFT_WHITE);
    _tft->setCursor(1, 1);
    _tft->print("Welcome to Page 2");
    _tft->setSwapBytes(true);
    // _tft->pushImage(39, 39, 50, 50, rainfall_50_outline);
    drawImage(rainfall_50_outline, 39, 39, 0, 0);
    // 绘制第二页的其他内容
}

void GUI::drawPage3() {
    _tft->fillScreen(TFT_WHITE);
    _tft->setCursor(1, 1);
    _tft->print("Welcome to Page 3");
    _tft->setSwapBytes(true);
    // _tft->pushImage(14, 14, 100, 100, rainfall_100_filled);
    drawImage(rainfall_100_filled, 100, 100, 14, 14);
    // 绘制第三页的其他内容
}

void GUI::drawPage4() {
    _tft->fillScreen(TFT_WHITE);
    _tft->setCursor(1, 1);
    _tft->print("Welcome to Page 4");
    _tft->setSwapBytes(true);
    // _tft->pushImage(14, 14, 100, 190, rainfall_100_outline);
    drawImage(rainfall_100_outline, 100, 100, 14, 14);
    // 绘制第四页的其他内容
}

void GUI::drawPage5() {
    _tft->fillScreen(TFT_BLACK);
    _tft->setCursor(10, 10);
    _tft->print("Welcome to Page 5");
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