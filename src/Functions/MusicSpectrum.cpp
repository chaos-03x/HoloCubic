#include "MusicSpectrum.h"

// MusicSpectrum 类的构造函数，初始化显示器（TFT）、FFT 对象，并设置频谱为不活跃状态。
MusicSpectrum::MusicSpectrum(TFT_eSPI *tft) : _tft(tft), _fft(), _active(false), _mode(0) {}

// 平滑和峰值衰减因子
#define SMOOTHING_FACTOR 0.7  // 频谱条变化的平滑因子
#define PEAK_DECAY 0.95       // 峰值标记的衰减因子

// 噪音过滤门限，低于此值的信号被认为是静音
#define NOISE_THRESHOLD 88

// 设置当前的频谱显示模式（例如：线性、圆形等）
void MusicSpectrum::setMode(int mode) {
    _mode = mode;
}

// 初始化 I2S 接口并清空屏幕，为频谱显示做准备
void MusicSpectrum::begin() {
    configureI2S();            // 设置 I2S 通信
    _tft->fillScreen(TFT_BLACK); // 清空显示屏
    _active = true;             // 将频谱标记为活跃状态
}

// 停止频谱显示并清空屏幕
void MusicSpectrum::stop() {
    _active = false;            // 将频谱标记为不活跃
    _tft->fillScreen(TFT_BLACK); // 清空显示屏
}

// 配置 I2S 接口来读取麦克风数据
void MusicSpectrum::configureI2S() {
    // I2S 配置结构体
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX), // I2S 主模式，接收数据
        .sample_rate = 44100,                               // 音频采样率
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,       // 16 位音频数据
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,        // 单通道（左声道）
        .communication_format = I2S_COMM_FORMAT_I2S,        // 标准 I2S 格式
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,           // 中断级别
        .dma_buf_count = 8,                                 // DMA 缓冲区数量
        .dma_buf_len = 64,                                  // 每个 DMA 缓冲区的长度
        .use_apll = false,                                  // 不使用 APLL
        .tx_desc_auto_clear = true,                         // 自动清除 DMA 缓冲区
        .fixed_mclk = 0                                     // 无固定主时钟
    };

    // I2S 引脚配置（SCK、WS 和 SD）
    i2s_pin_config_t pin_config = {
        .bck_io_num = 15,   // SCK
        .ws_io_num = 14,    // WS
        .data_out_num = -1, // 不使用（没有输出）
        .data_in_num = 13   // SD（麦克风数据输入）
    };

    // 安装并设置 I2S 驱动
    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &pin_config);
}

// 使用 I2S 读取麦克风数据，并处理它们以进行 FFT
void MusicSpectrum::readMicData() {
    size_t bytesRead;
    int16_t i2sData[samples];   // 存储 I2S 原始音频数据的缓冲区

    // 从 I2S 读取数据（阻塞直到有数据）
    i2s_read(I2S_NUM_0, &i2sData, sizeof(i2sData), &bytesRead, portMAX_DELAY);

    // 将 I2S 数据转换为 FFT 的输入
    for (int i = 0; i < samples; i++) {
        vReal[i] = (double)i2sData[i];  // 将音频样本存储为实数部分
        vImag[i] = 0;                   // 虚数部分为 0（实际信号）

        // 应用噪声过滤，低于门限的信号设置为 0
        if (vReal[i] < NOISE_THRESHOLD) {
            vReal[i] = 0;  // 消除噪声，将小于门限的值设为 0
        }
    }
}

// 主更新函数，用于刷新频谱显示
void MusicSpectrum::update() {
    if (!_active) return;  // 如果频谱未激活，则退出

    readMicData();  // 读取来自麦克风的新数据

    // 对数据进行 FFT 计算，得到频率成分
    _fft.Compute(vReal, vImag, samples, FFT_FORWARD);

    // 根据当前模式绘制频谱
    drawSpectrum();
}

// 根据选择的模式绘制音频频谱（当前为线性频谱）
void MusicSpectrum::drawSpectrum() {
    drawLinearSpectrum();  // 目前只实现了线性频谱
}

// 绘制具有 32 个频谱条的线性频谱
void MusicSpectrum::drawLinearSpectrum() {
    const int barWidth = _tft->width() / 32;  // 每个频谱条的宽度（总共 32 个条）

    // 遍历 32 个频率带
    for (int i = 0; i < 32; i++) {
        float magnitude = vReal[i];  // 获取该频段的幅度

        // 对频谱条进行平滑处理（渐变过渡）
        if (magnitude > bar_chart[i]) {
            bar_chart[i] = magnitude;  // 如果幅度增加，立即更新
        } else {
            bar_chart[i] = SMOOTHING_FACTOR * bar_chart[i] + 
                           (1.0 - SMOOTHING_FACTOR) * magnitude;  // 平滑下降
        }

        // 更新峰值
        if (magnitude > bar_chart_peaks[i]) {
            bar_chart_peaks[i] = magnitude;  // 如果幅度更高，更新峰值
        } else {
            bar_chart_peaks[i] *= PEAK_DECAY;  // 峰值逐渐衰减
        }

        // 将幅度映射到屏幕高度（频谱条和峰值）
        int barHeight = map(bar_chart[i], 0, 500, 0, _tft->height());
        int peakHeight = map(bar_chart_peaks[i], 0, 500, 0, _tft->height());

        // 如果当前条的高度低于上一个条，则擦除旧条
        if (barHeight < last_bar_height[i]) {
            _tft->fillRect(i * barWidth, _tft->height() - last_bar_height[i], 
                           barWidth, last_bar_height[i] - barHeight, TFT_BLACK);
        }

        // 绘制新的频谱条
        uint32_t color = _tft->color565(i * 8, 255 - (i * 8), 128);  // 根据索引生成颜色
        _tft->fillRect(i * barWidth, _tft->height() - barHeight, 
                       barWidth, barHeight, color);

        // 擦除旧的峰值标记
        if (last_peak_height[i] > peakHeight) {
            _tft->drawLine(i * barWidth, _tft->height() - last_peak_height[i], 
                           (i + 1) * barWidth, _tft->height() - last_peak_height[i], TFT_BLACK);
        }

        // 绘制新的峰值标记
        _tft->drawLine(i * barWidth, _tft->height() - peakHeight, 
                       (i + 1) * barWidth, _tft->height() - peakHeight, TFT_RED);

        // 更新缓存中的条高度和峰值高度
        last_bar_height[i] = barHeight;
        last_peak_height[i] = peakHeight;
    }
}

// void MusicSpectrum::drawLinearSpectrum() {
//     const int barWidth = _tft->width() / 32;  // 32个频谱条
//     for (int i = 0; i < 32; i++) {
//         float magnitude = vReal[i];  // 获取FFT的结果
        
//         // 使用平滑因子平滑频谱条
//         if (magnitude > bar_chart[i]) {
//             bar_chart[i] = magnitude;  // 如果当前频率值大于上次的值，立即增长
//         } else {
//             bar_chart[i] = SMOOTHING_FACTOR * bar_chart[i] + (1.0 - SMOOTHING_FACTOR) * magnitude;  // 否则逐渐下降
//         }

//         // 峰值也需要逐渐衰减
//         if (magnitude > bar_chart_peaks[i]) {
//             bar_chart_peaks[i] = magnitude;  // 更新峰值
//         } else {
//             bar_chart_peaks[i] *= PEAK_DECAY;  // 逐渐衰减峰值
//         }

//         // 绘制频谱条
//         int barHeight = map(bar_chart[i], 0, 500, 0, _tft->height());  // 将频谱值映射到屏幕高度
//         int peakHeight = map(bar_chart_peaks[i], 0, 500, 0, _tft->height());

//         // 根据频谱条高度设置渐变颜色
//         uint32_t color = _tft->color565(i * 8, 255 - (i * 8), 128);  // 简单的RGB渐变
//         _tft->fillRect(i * barWidth, _tft->height() - barHeight, barWidth, barHeight, color);

//         // 绘制峰值标记
//         _tft->drawLine(i * barWidth, _tft->height() - peakHeight, (i + 1) * barWidth, _tft->height() - peakHeight, TFT_RED);
//     }
// }