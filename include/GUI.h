#ifndef GUI_H
#define GUI_H

#include <TFT_eSPI.h>
#include <vector>
#include "WeatherAPI.h"

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

    unsigned long lastPageChangeTime = 0; // 记录上次翻页的时间
    const unsigned long pageChangeDelay = 300; // 翻页延迟时间（毫秒）

private:
    TFT_eSPI* _tft;
    WeatherAPI& _weatherAPI;
    
    enum class PageType { PARENT, CHILD };
    struct Page {
        PageType type;
        std::vector<Page> subPages; // 存储子页面
    };

    std::vector<Page> _pages; // 存储页面顺序
    int _currentPage;         // 当前父页面索引
    int _currentSubPage;      // 当前子页面索引
    bool _inSubPage;          // 是否在子页面

    int _xValue; // 摇杆X轴输入
    int _yValue; // 摇杆Y轴输入
    int _zValue; // 摇杆Z轴输入

    // 其他可能的辅助函数
    void drawPage(Page page);
    void drawParentPage(int index);
    void drawChildPage(int index); // 修改为更直观的名称
    void drawPage1();
    void drawPage2();
    void drawPage3();
    void drawPage4();
    void drawPage5();
    void drawPage1_1();
    void drawPage2_1();
    void drawPage3_1();
    void drawPage4_1();
    void drawPage5_1();

};



#endif // GUI_H
