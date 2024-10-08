#include <pgmspace.h>

// Part1. 点阵数据
// PROGMEM 定义使数据存储在单片机的ROM中

const unsigned char HZ_fu PROGMEM[] = 
{
    0x40,0x00,0x23,0xFE,0x20,0x00,0xF9,0xFC,0x11,0x04,0x11,0x04,0x21,0xFC,0x30,0x00,
0x6B,0xFE,0xAA,0x22,0x22,0x22,0x23,0xFE,0x22,0x22,0x22,0x22,0x23,0xFE,0x22,0x02,/*"福",0*/
};
const unsigned char HZ_zhou PROGMEM[] = 
{
    0x10,0x04,0x10,0x84,0x10,0x84,0x10,0x84,0x10,0x84,0x54,0xA4,0x52,0x94,0x52,0x94,
0x90,0x84,0x10,0x84,0x10,0x84,0x10,0x84,0x20,0x84,0x20,0x84,0x40,0x04,0x80,0x04,/*"州",1*/
};

const unsigned char HZ_bao PROGMEM[] = 
{ 
0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0xFC,0x1F,0xFC,0x1D,0xDC,0x02,0x20,0x3F,0xFE,
0x7F,0xFF,0x1C,0x1C,0x60,0x83,0x0E,0xB8,0x33,0x87,0x00,0x00,0x00,0x00,0x00,0x00,/*"暴",0*/
};

const unsigned char HZ_da PROGMEM[] = 
{ 
0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x80,0x00,0x80,0x7F,0xFF,0x01,0x40,0x01,0x40,
0x02,0x20,0x04,0x18,0x18,0x04,0x60,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"大",1*/
};

const unsigned char HZ_zhong PROGMEM[] = 
{ 
0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x80,0x3F,0xFE,0x20,0x82,0x20,0x82,0x3F,0xFE,
0x20,0x82,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"中",2*/
};

const unsigned char HZ_xiao PROGMEM[] = 
{ 
0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x80,0x18,0x8C,0x10,0x84,0x10,0x82,0x20,0x82,
0x20,0x81,0x40,0x81,0x00,0x80,0x07,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"小",3*/
};

const unsigned char HZ_lei PROGMEM[] = 
{ 
0x00,0x00,0x00,0x00,0x3F,0xFE,0x7F,0xFF,0x40,0x81,0x40,0x81,0x1E,0x3C,0x3F,0xFE,
0x10,0x84,0x1F,0xFC,0x10,0x84,0x1F,0xFE,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"雷",4*/
};

const unsigned char HZ_zhen PROGMEM[] = 
{ 
0x00,0x00,0x00,0x00,0x7C,0x20,0x65,0xFF,0x68,0x40,0x68,0x90,0x69,0xFE,0x68,0x10,
0x64,0x10,0x69,0xFF,0x60,0x10,0x60,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"阵",5*/
};

const unsigned char HZ_yu PROGMEM[] = 
{ 
0x00,0x00,0x00,0x00,0x7F,0xFF,0x00,0x80,0x3F,0xFE,0x20,0x82,0x2C,0xB2,0x22,0x8A,
0x28,0xA2,0x26,0x9A,0x20,0x82,0x20,0x9E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"雨",6*/
};

const unsigned char HZ_yin PROGMEM[] = 
{ 
0x00,0x00,0x00,0x00,0x7E,0xFE,0x22,0x82,0x24,0x82,0x24,0xFE,0x2C,0xC2,0x24,0x82,
0x22,0xFE,0x2C,0x82,0x21,0x02,0x63,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"阴",7*/
};

const unsigned char HZ_duo PROGMEM[] = 
{ 
0x00,0x00,0x00,0x00,0x01,0xC0,0x1E,0x78,0x26,0x30,0x01,0xC0,0x1E,0x10,0x00,0xFF,
0x1F,0x06,0x00,0x18,0x00,0xE0,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"多",8*/
};

const unsigned char HZ_yun PROGMEM[] = 
{ 
0x00,0x00,0x00,0x00,0x1F,0xFE,0x00,0x00,0x00,0x00,0x7E,0xFF,0x01,0x00,0x02,0x00,
0x04,0x18,0x08,0x04,0x10,0xFC,0x3F,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"云",9*/
};

const unsigned char HZ_qing PROGMEM[] = 
{ 
0x00,0x00,0x00,0x00,0x19,0xFF,0x64,0x10,0x44,0xFE,0x45,0xFF,0x7C,0x00,0x44,0xFE,
0x44,0xFE,0x7C,0xFE,0x64,0x82,0x00,0x8E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"晴",10*/
};



//Part.2 字模结构体
struct CN_character
{
  char  Index[4]; // 汉字内码索引,在UTF-8编码下，每个汉字占3个字节，第四个是结束符0
  const unsigned char* HZ_Id;  // 点阵图
  unsigned char HZ_width;    // 字体宽度
};

// 定义结构数组
PROGMEM const CN_character hanzi[] =
{
    {"福", HZ_fu, 16},
    {"州", HZ_zhou, 16},
    {"暴", HZ_bao, 16},
    {"大", HZ_da, 16},
    {"中", HZ_zhong, 16},
    {"小", HZ_xiao, 16},
    {"雷", HZ_lei, 16},
    {"阵", HZ_zhen, 16},
    {"雨", HZ_yu, 16},
    {"阴", HZ_yin, 16},
    {"多", HZ_duo, 16},
    {"云", HZ_yun, 16},
    {"晴", HZ_qing, 16}
};
