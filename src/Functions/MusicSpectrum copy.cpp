// #include "MusicSpectrum.h"

// MusicSpectrum::MusicSpectrum(TFT_eSPI *tft) : _tft(tft), _fft(), _active(false), _mode(0) {}

// #define NOISE_THRESHOLD 50  // 根据实际情况调整，800是一个初始值

// void MusicSpectrum::setMode(int mode) {
//     _mode = mode;
// }

// void MusicSpectrum::begin() {
//     configureI2S();
//     _tft->fillScreen(TFT_BLACK);
//     _active = true;
// }

// void MusicSpectrum::stop() {
//     _active = false;
//     _tft->fillScreen(TFT_BLACK);  // 清空屏幕
// }

// void MusicSpectrum::configureI2S() {
//     // I2S 配置
//     i2s_config_t i2s_config = {
//         .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX), 
//         .sample_rate = 44100,
//         .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
//         .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
//         .communication_format = I2S_COMM_FORMAT_I2S,
//         .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
//         .dma_buf_count = 8,
//         .dma_buf_len = 64,
//         .use_apll = false,
//         .tx_desc_auto_clear = true,
//         .fixed_mclk = 0
//     };
    
//     i2s_pin_config_t pin_config = {
//         .bck_io_num = 15,   // SCK
//         .ws_io_num = 14,    // WS
//         .data_out_num = -1, // 未使用
//         .data_in_num = 13   // SD
//     };
    
//     i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
//     i2s_set_pin(I2S_NUM_0, &pin_config);
// }

// void MusicSpectrum::readMicData() {
//     size_t bytesRead;
//     int16_t i2sData[samples];
//     i2s_read(I2S_NUM_0, &i2sData, sizeof(i2sData), &bytesRead, portMAX_DELAY);
    
//     for (int i = 0; i < samples; i++) {
//         vReal[i] = (double)i2sData[i];
//         vImag[i] = 0;
//         // 噪声门限处理，低于门限值的信号视为0
//         if (vReal[i] < NOISE_THRESHOLD) {
//             vReal[i] = 0;
//         }
//     }
// }

// void MusicSpectrum::update() {
//     if (!_active) return;
    
//     readMicData();
    
//     _fft.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
//     _fft.Compute(vReal, vImag, samples, FFT_FORWARD);
//     _fft.ComplexToMagnitude(vReal, vImag, samples);

//     drawSpectrum();
// }

// void MusicSpectrum::drawSpectrum() {
//     _tft->fillScreen(TFT_BLACK);  // 每次绘制前清空屏幕

//     switch (_mode) {
//         case 0:
//             drawLinearSpectrum();
//             break;
//         case 1:
//             drawCircularSpectrum();
//             break;
//         case 2:
//             drawSymmetricalSpectrum();
//             break;
//         case 3:
//             drawLinearSpectrumAStyle();
//             break;
//         default:
//             drawLinearSpectrum();
//             break;
//     }
// }

// void MusicSpectrum::drawLinearSpectrum() {
//     static float colorOffset = 0;  // 用于控制颜色的动态变化
//     int numberOfBars = 32;         // 频谱条数量
//     int gap = 2;                   // 频谱条之间的间隙，单位：像素

//     // 计算频谱条的实际宽度，减去总间隙后平均分配
//     int totalGapWidth = (numberOfBars - 1) * gap;
//     int barWidth = (_tft->width() - totalGapWidth) / numberOfBars;

//     for (int i = 0; i < numberOfBars; i++) {
//         // 对数缩放，平衡频谱，避免频谱条过于满屏
//         float weightedValue = log10(vReal[i] + 1) * 15;  // 调整缩放参数让频谱有落差
//         int barHeight = (int)(weightedValue);            // 使用 weightedValue 计算高度

//         // 确保 barHeight 在屏幕范围内
//         if (barHeight > _tft->height()) {
//             barHeight = _tft->height();
//         }

//         // 颜色随时间渐变，保持频谱整体颜色一致且随时间变化
//         float timeFactor = (colorOffset) * 2 * PI;
//         uint8_t red = (uint8_t)(sin(timeFactor) * 127 + 128);
//         uint8_t green = (uint8_t)(sin(timeFactor + 2 * PI / 3) * 127 + 128);
//         uint8_t blue = (uint8_t)(sin(timeFactor + 4 * PI / 3) * 127 + 128);
//         uint16_t color = _tft->color565(red, green, blue);  // 生成颜色

//         // 计算当前频谱条的 x 坐标，考虑间隙
//         int x = i * (barWidth + gap);

//         // 绘制频谱条
//         _tft->fillRect(x, _tft->height() - barHeight, barWidth, barHeight, color);
//     }

//     // 更新颜色偏移，控制整个频谱的颜色动态变化
//     colorOffset += 0.01;  // 减小步长，让颜色渐变更加平滑
//     if (colorOffset > 2 * PI) colorOffset = 0;
// }


// void MusicSpectrum::drawCircularSpectrum() {
//     static float colorOffset = 0;  // 控制颜色动态变化
//     int centerX = _tft->width() / 2;
//     int centerY = _tft->height() / 2;
//     int baseRadius = min(centerX, centerY) / 4;  // 缩小基础圆的半径
//     int waveAmplitude = 10;  // 控制波形振幅
    
//     int numSegments = 100;  // 增加频谱分量数量，形成更加连贯的波形
//     float angleStep = TWO_PI / numSegments;

//     for (int i = 0; i < numSegments; i++) {
//         float angle = i * angleStep;  // 将64条频谱均匀分布在圆周上
        
//         // 计算频谱条长度，并用波形（波峰波谷）表示
//         float barLength = log10(vReal[i % 32] + 1) * waveAmplitude;  // 使用对数缩放，避免频谱过满
//         float xOuter = centerX + (baseRadius + barLength) * cos(angle);
//         float yOuter = centerY + (baseRadius + barLength) * sin(angle);
//         float xInner = centerX + baseRadius * cos(angle);
//         float yInner = centerY + baseRadius * sin(angle);
        
//         // 颜色渐变，随时间动态变化
//         float timeFactor = (colorOffset + (float)i / numSegments) * 2 * PI;
//         uint8_t red = (uint8_t)(sin(timeFactor) * 127 + 128);
//         uint8_t green = (uint8_t)(sin(timeFactor + 2 * PI / 3) * 127 + 128);
//         uint8_t blue = (uint8_t)(sin(timeFactor + 4 * PI / 3) * 127 + 128);
//         uint16_t color = _tft->color565(red, green, blue);

//         // 绘制波形频谱，从圆周向外扩展，形成连贯的波形
//         _tft->drawLine(xInner, yInner, xOuter, yOuter, color);
//     }
    
//     // 更新颜色偏移，控制颜色随时间变化
//     colorOffset += 0.01;
//     if (colorOffset > 1) colorOffset = 0;
// }

// void MusicSpectrum::drawSymmetricalSpectrum() {
//     int numberOfBars = 16;  // 左侧（或右侧）频谱条数量
//     int gap = 2;            // 频谱条之间的间隙，单位：像素

//     // 计算频谱条的实际宽度
//     int totalGapWidth = (numberOfBars * 2 - 1) * gap;  // 左右各 numberOfBars 个频谱条
//     int barWidth = (_tft->width() - totalGapWidth) / (numberOfBars * 2);

//     for (int i = 0; i < numberOfBars; i++) {
//         // 对数放缩频率分量，避免低频过大
//         float weightedValue = log10(vReal[i] + 1) * 15;  // 调整缩放参数
//         int barHeight = (int)(weightedValue);

//         // 确保 barHeight 在屏幕范围内
//         if (barHeight > _tft->height() / 2) {
//             barHeight = _tft->height() / 2;
//         }

//         // 计算左侧频谱条的 x 坐标
//         int xLeft = i * (barWidth + gap);

//         // 计算右侧频谱条的 x 坐标
//         int xRight = _tft->width() - (i + 1) * (barWidth + gap) + gap;

//         // 绘制左侧频谱条（上半部分和下半部分）
//         _tft->fillRect(xLeft, _tft->height() / 2 - barHeight, barWidth, barHeight, TFT_CYAN);  // 上半部分
//         _tft->fillRect(xLeft, _tft->height() / 2 + 1, barWidth, barHeight, TFT_CYAN);          // 下半部分

//         // 绘制右侧频谱条（上半部分和下半部分）
//         _tft->fillRect(xRight, _tft->height() / 2 - barHeight, barWidth, barHeight, TFT_CYAN);  // 上半部分
//         _tft->fillRect(xRight, _tft->height() / 2 + 1, barWidth, barHeight, TFT_CYAN);          // 下半部分
//     }
// }


