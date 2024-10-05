#include <cstdint>
#include <TFT_eSPI.h>
#include "PicMap.h"

/* 输出透明像素 */
const uint16_t TRANSPARENT_COLOR = 0xF81F;  // 用紫色表示透明像素

void drawImage(const unsigned short* data, int width, int height, int xOffset, int yOffset) {
    TFT_eSPI tft = TFT_eSPI();
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            uint16_t color = data[y * width + x];
            if (color != TRANSPARENT_COLOR) { // 检查是否为透明像素
                tft.drawPixel(xOffset + x, yOffset + y, color); // 绘制非透明像素
            }
        }
    }
}