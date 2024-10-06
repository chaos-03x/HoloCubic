#include <cstdint>
#include <WString.h>

#ifndef FONTMANAGER_H
#define FONTMANAGER_H

void showMyFont(TFT_eSPI* tft, int32_t x, int32_t y, const char c[3], uint32_t color);
void showMyFonts(TFT_eSPI* tft, int32_t x, int32_t y, const char str[], uint32_t color, bool vertical = false);
void showtext(TFT_eSPI* tft, int16_t x,int16_t y,uint8_t font,uint8_t s,uint16_t fg,uint16_t bg,const String str);

#endif // FONTMANAGER_H
