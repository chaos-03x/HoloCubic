#include <TFT_eSPI.h>
#include <FontMap.h>

// 绘制单个汉字
void showMyFont(TFT_eSPI* tft, int32_t x, int32_t y, const char c[3], uint32_t color) {
int hanzi_len = sizeof(hanzi) / sizeof(hanzi[0]); // 获取字库内有多少汉字

for (int i = 0; i < hanzi_len; i++) // 遍历字库，查找与参数c匹配的汉字
if (hanzi[i].Index[0] == c[0] && hanzi[i].Index[1] == c[1] && hanzi[i].Index[2] == c[2]) // 在字库中，hanzi[k].Index 存储了每个汉字的编码。通过比较 c 数组中的字符与字库中的字符进行匹配。
{
  tft->drawBitmap(x, y, hanzi[i].HZ_Id, hanzi[i].HZ_width, 16, color); // 绘制汉字，输入参数依次是:x轴、y轴、位图指针、宽度、高度、颜色
}
}

// 显示多个汉字，并控制绘制方向
void showMyFonts(TFT_eSPI* tft, int32_t x, int32_t y, const char str[], uint32_t color, bool vertical = false) {
  for (int i = 0; i < strlen(str); i += 3) {
    showMyFont(tft, x, y, str+i, color);
    if (vertical){
      y += 17; // 垂直绘制，增加y坐标
    } else {
      x += 17; // 水平绘制，增加x坐标
    }
  }
}

void showtext(TFT_eSPI* tft, int16_t x,int16_t y,uint8_t font,uint8_t s,uint16_t fg,uint16_t bg,const String str)
{
  //设置文本显示坐标，和文本的字体，默认以左上角为参考点，
    tft->setCursor(x, y, font);
  // 设置文本颜色为白色，文本背景黑色
    tft->setTextColor(fg,bg);
  //设置文本大小，文本大小的范围是1-7的整数
    tft->setTextSize(s);
  // 设置显示的文字，注意这里有个换行符 \n 产生的效果
    tft->println(str);
}

