#ifndef GUI_H
#define GUI_H

#include <TFT_eSPI.h>
#include <vector>
#include "WeatherAPI.h"
#include "MusicSpectrum.h"

class GUI {
public:
    GUI(TFT_eSPI* tft, WeatherAPI& weatherAPI); 
    void init();
    void draw();
    void drawPage0();
    void handleInput();
    void update();  // 更新页面内容
    void updateJoystick(int x, int y, int z);
    void ConnectingGUI();
    int getCurrentPage() {
        return _currentPage;
    }
    MusicSpectrum *spectrumAnalyzer = nullptr;  // 用指针管理 SpectrumAnalyzer 实例
    
    unsigned long lastPageChangeTime = 0; // 记录上次翻页的时间
    const unsigned long pageChangeDelay = 300; // 翻页延迟时间（毫秒）

private:
    TFT_eSPI* _tft;
    WeatherAPI& _weatherAPI;

    enum class PageType { PARENT };
    struct Page {
        PageType type;
    };

    std::vector<Page> _pages; // 存储页面顺序
    int _currentPage;         // 当前页面索引
    int _currentMode = 0;      // 频谱模式索引

    int _xValue; // 摇杆X轴输入
    int _yValue; // 摇杆Y轴输入
    int _zValue; // 摇杆Z轴输入

    // Page绘制（优化）
    void drawParentPage(int index);
    void drawPage1();
    void drawPage2();
    void drawPage3();
    void drawPage4();
    void drawPage5();
};

#endif // GUI_H
