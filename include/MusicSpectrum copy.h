#ifndef MUSIC_SPECTRUM_H
#define MUSIC_SPECTRUM_H

#include <TFT_eSPI.h>
#include <driver/i2s.h>
#include <arduinoFFT.h>

class MusicSpectrum {
public:
    MusicSpectrum(TFT_eSPI *tft);  // 构造函数
    void begin();                  // 初始化麦克风和I2S
    void update();                 // 更新频谱
    void stop();                   // 停止频谱更新
    void setMode(int mode);        // 设置频谱模式（线性，圆周，对称）

private:
    TFT_eSPI *_tft;                // 显示对象
    arduinoFFT _fft;               // FFT对象
    bool _active;                  // 是否活跃
    int _mode;                     // 当前的频谱模式

    void configureI2S();           // 配置I2S
    void readMicData();            // 读取麦克风数据
    void drawSpectrum();           // 绘制频谱
    void drawLinearSpectrum();      // 绘制线性频谱
    void drawCircularSpectrum();    // 绘制圆周频谱
    void drawSymmetricalSpectrum(); // 绘制对称频谱

    static const int samples = 512;  // FFT 样本数量
    double vReal[samples];           // 真实部分
    double vImag[samples];           // 虚部
    double vTemp[samples];       // 临时存储频谱数据
    static const int NumofCopy = samples * sizeof(double); // 用于复制数据的大小
};

#endif
