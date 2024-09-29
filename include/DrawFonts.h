#include <TFT_eSPI.h>
#include <MyFont.h>
TFT_eSPI tft = TFT_eSPI();

// 绘制单个汉字
void showMyFont(int32_t x, int32_t y, const char c[3], uint32_t color) {
int hanzi_len = sizeof(hanzi) / sizeof(hanzi[0]); // 获取字库内有多少汉字

for (int i = 0; i < hanzi_len; i++) // 遍历字库，查找与参数c匹配的汉字
if (hanzi[i].Index[0] == c[0] && hanzi[i].Index[1] == c[1] && hanzi[i].Index[2] == c[2]) // 在字库中，hanzi[k].Index 存储了每个汉字的编码。通过比较 c 数组中的字符与字库中的字符进行匹配。
{
tft.drawBitmap(x, y, hanzi[i].CH_Id, hanzi[i].CH_width, 16, color); // 绘制汉字，输入参数依次是:x轴、y轴、位图指针、宽度、高度、颜色
}
}

// 显示多个汉字，并控制绘制方向
void showMyFonts(int32_t x, int32_t y, const char str[], uint32_t color, bool vertical = false) {
for (int i = 0; i < strlen(str); i += 3) {
showMyFont(x, y, str+i, color);

if (vertical){
  y += 17; // 垂直绘制，增加y坐标
} else {
  x += 17; // 水平绘制，增加x坐标
}
}
}
