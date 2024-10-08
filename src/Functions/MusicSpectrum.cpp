#include "MusicSpectrum.h"

MusicSpectrum::MusicSpectrum(TFT_eSPI *tft) : _tft(tft), _fft(), _active(false), _mode(0) {}
// 平滑参数
#define SMOOTHING_FACTOR 0.7  // 平滑因子
#define PEAK_DECAY 0.95  // 峰值衰减因子
#define NOISE_THRESHOLD 88  // 根据实际情况调整

void MusicSpectrum::setMode(int mode) {
    _mode = mode;
}

void MusicSpectrum::begin() {
    configureI2S();
    _tft->fillScreen(TFT_BLACK);
    _active = true;
}

void MusicSpectrum::stop() {
    _active = false;
    _tft->fillScreen(TFT_BLACK);  // 清空屏幕
}

void MusicSpectrum::configureI2S() {
    // I2S 配置
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX), 
        .sample_rate = 44100,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 8,
        .dma_buf_len = 64,
        .use_apll = false,
        .tx_desc_auto_clear = true,
        .fixed_mclk = 0
    };
    
    i2s_pin_config_t pin_config = {
        .bck_io_num = 15,   // SCK
        .ws_io_num = 14,    // WS
        .data_out_num = -1, // 未使用
        .data_in_num = 13   // SD
    };
    
    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &pin_config);
}

void MusicSpectrum::readMicData() {
    size_t bytesRead;
    int16_t i2sData[samples];
    i2s_read(I2S_NUM_0, &i2sData, sizeof(i2sData), &bytesRead, portMAX_DELAY);
    
    for (int i = 0; i < samples; i++) {
        vReal[i] = (double)i2sData[i];
        vImag[i] = 0;
        // 噪声门限处理，低于门限值的信号视为0
        if (vReal[i] < NOISE_THRESHOLD) {
            vReal[i] = 0;
        }
    }
}

void MusicSpectrum::update() {
    if (!_active) return;
    
    readMicData();

    _fft.Compute(vReal, vImag, samples, FFT_FORWARD);

    drawSpectrum();
}

void MusicSpectrum::drawSpectrum() {
    _tft->fillScreen(TFT_BLACK);  // 每次绘制前清空屏幕

    switch (_mode) {
        case 0:
            drawLinearSpectrum();            // 绘制线性频谱
            break;
        case 1:
            // drawSymmetricalSpectrum();       // 绘制对称频谱
            drawLinearSpectrum();            // 绘制线性频谱
            break;
        case 2:
            // void drawWavySpectrum();         // 绘制波浪频谱
            drawLinearSpectrum();            // 绘制线性频谱
            break;
        case 3:
            // void drawRadialSpectrum();       // 绘制放射频谱
            // drawSymmetricalSpectrum();       // 绘制对称频谱
            drawLinearSpectrum();            // 绘制线性频谱
        default:
            drawLinearSpectrum();
            break;
    }
}

void MusicSpectrum::drawLinearSpectrum() {
    const int barWidth = _tft->width() / 32;  // 32个频谱条
    for (int i = 0; i < 32; i++) {
        float magnitude = vReal[i];  // 获取FFT的结果
        
        // 使用平滑因子平滑频谱条
        if (magnitude > bar_chart[i]) {
            bar_chart[i] = magnitude;  // 如果当前频率值大于上次的值，立即增长
        } else {
            bar_chart[i] = SMOOTHING_FACTOR * bar_chart[i] + (1.0 - SMOOTHING_FACTOR) * magnitude;  // 否则逐渐下降
        }

        // 峰值也需要逐渐衰减
        if (magnitude > bar_chart_peaks[i]) {
            bar_chart_peaks[i] = magnitude;  // 更新峰值
        } else {
            bar_chart_peaks[i] *= PEAK_DECAY;  // 逐渐衰减峰值
        }

        // 绘制频谱条
        int barHeight = map(bar_chart[i], 0, 500, 0, _tft->height());  // 将频谱值映射到屏幕高度
        int peakHeight = map(bar_chart_peaks[i], 0, 500, 0, _tft->height());

        // 根据频谱条高度设置渐变颜色
        uint32_t color = _tft->color565(i * 8, 255 - (i * 8), 128);  // 简单的RGB渐变
        _tft->fillRect(i * barWidth, _tft->height() - barHeight, barWidth, barHeight, color);

        // 绘制峰值标记
        _tft->drawLine(i * barWidth, _tft->height() - peakHeight, (i + 1) * barWidth, _tft->height() - peakHeight, TFT_RED);
    }
}


void MusicSpectrum::drawSymmetricalSpectrum() {
    int numberOfBars = 16;  // 左侧（或右侧）频谱条数量
    int gap = 2;            // 频谱条之间的间隙，单位：像素

    // 计算频谱条的实际宽度
    int totalGapWidth = (numberOfBars * 2 - 1) * gap;  // 计算除中间间隙外的总间隙
    int barWidth = (_tft->width() - totalGapWidth) / (numberOfBars * 2);  // 左右对称频谱条

    // 确保 barWidth 是正数
    if (barWidth <= 0) {
        Serial.println("Bar width calculation error!");  // 输出调试信息
        return;  // 退出函数
    }

    for (int i = 0; i < numberOfBars; i++) {
        // 对数放缩频率分量，避免低频过大
        float magnitude = log10(vReal[i] + 1) * 15;  // 调整缩放参数

        // 平滑左侧和右侧频谱条的值
        if (magnitude > bar_chart[i]) {
            bar_chart[i] = magnitude;  // 左侧频谱条立即增长
        } else {
            bar_chart[i] = SMOOTHING_FACTOR * bar_chart[i] + (1.0 - SMOOTHING_FACTOR) * magnitude;  // 左侧频谱条逐渐下降
        }

        if (magnitude > bar_chart_peaks[i]) {
            bar_chart_peaks[i] = magnitude;  // 左侧频谱条的峰值
        } else {
            bar_chart_peaks[i] *= PEAK_DECAY;  // 左侧频谱条的峰值逐渐衰减
        }

        // 右侧频谱条使用相同的平滑逻辑
        if (magnitude > bar_chart[numberOfBars + i]) {
            bar_chart[numberOfBars + i] = magnitude;  // 右侧频谱条立即增长
        } else {
            bar_chart[numberOfBars + i] = SMOOTHING_FACTOR * bar_chart[numberOfBars + i] + (1.0 - SMOOTHING_FACTOR) * magnitude;  // 右侧频谱条逐渐下降
        }

        if (magnitude > bar_chart_peaks[numberOfBars + i]) {
            bar_chart_peaks[numberOfBars + i] = magnitude;  // 右侧频谱条的峰值
        } else {
            bar_chart_peaks[numberOfBars + i] *= PEAK_DECAY;  // 右侧频谱条的峰值逐渐衰减
        }

        // 映射频谱条高度
        int barHeightLeft = map(bar_chart[i], 0, 500, 0, _tft->height() / 2);
        int peakHeightLeft = map(bar_chart_peaks[i], 0, 500, 0, _tft->height() / 2);

        int barHeightRight = map(bar_chart[numberOfBars + i], 0, 500, 0, _tft->height() / 2);
        int peakHeightRight = map(bar_chart_peaks[numberOfBars + i], 0, 500, 0, _tft->height() / 2);

        // 计算左侧频谱条的 x 坐标
        int xLeft = i * (barWidth + gap);

        // 计算右侧频谱条的 x 坐标
        int xRight = _tft->width() - (i + 1) * (barWidth + gap);

        // 绘制左侧频谱条
        _tft->fillRect(xLeft, _tft->height() / 2 - barHeightLeft, barWidth, barHeightLeft, TFT_CYAN);  // 上半部分
        _tft->fillRect(xLeft, _tft->height() / 2 + 1, barWidth, barHeightLeft, TFT_CYAN);              // 下半部分

        // 绘制左侧峰值标记
        _tft->drawLine(xLeft, _tft->height() / 2 - peakHeightLeft, xLeft + barWidth, _tft->height() / 2 - peakHeightLeft, TFT_RED);

        // 绘制右侧频谱条
        _tft->fillRect(xRight, _tft->height() / 2 - barHeightRight, barWidth, barHeightRight, TFT_CYAN);  // 上半部分
        _tft->fillRect(xRight, _tft->height() / 2 + 1, barWidth, barHeightRight, TFT_CYAN);               // 下半部分

        // 绘制右侧峰值标记
        _tft->drawLine(xRight, _tft->height() / 2 - peakHeightRight, xRight + barWidth, _tft->height() / 2 - peakHeightRight, TFT_RED);
    }
}



void MusicSpectrum::drawWavySpectrum() {
    const int barCount = 32;
    const int barWidth = _tft->width() / barCount;

    for (int i = 0; i < barCount; i++) {
        float magnitude = vReal[i];
        bar_chart[i] = (magnitude > bar_chart[i]) ? magnitude : SMOOTHING_FACTOR * bar_chart[i] + (1.0 - SMOOTHING_FACTOR) * magnitude;

        int waveOffset = (int)(sin(i + millis() / 200.0) * 10);  // 产生波动的偏移量
        int barHeight = map(bar_chart[i], 0, 500, 0, _tft->height() / 2) + waveOffset;

        uint32_t color = _tft->color565(128 + i * 4, 128 - i * 4, 255 - i * 8);  // 渐变颜色
        _tft->fillRect(i * barWidth, _tft->height() / 2 - barHeight, barWidth, barHeight, color);
    }
}

void MusicSpectrum::drawRadialSpectrum() {
    const int centerX = _tft->width() / 2;
    const int centerY = _tft->height() / 2;
    const int maxRadius = min(centerX, centerY);

    for (int i = 0; i < 16; i++) {
        float magnitude = vReal[i];
        bar_chart[i] = (magnitude > bar_chart[i]) ? magnitude : SMOOTHING_FACTOR * bar_chart[i] + (1.0 - SMOOTHING_FACTOR) * magnitude;

        int barLength = map(bar_chart[i], 0, 500, 0, maxRadius);
        float angle = i * (PI / 8);  // 16条频谱，每条间隔22.5度

        int x1 = centerX + cos(angle) * barLength;
        int y1 = centerY + sin(angle) * barLength;
        int x2 = centerX + cos(angle + PI) * barLength;
        int y2 = centerY + sin(angle + PI) * barLength;

        uint32_t color = _tft->color565(255 - i * 16, 100 + i * 10, 200 - i * 10);  // 环状渐变
        _tft->drawLine(centerX, centerY, x1, y1, color);
        _tft->drawLine(centerX, centerY, x2, y2, color);
    }
}

