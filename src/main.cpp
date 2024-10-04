#include <SPI.h> //导入库
#include <TFT_eSPI.h>
#include "lvgl.h"
#include "lv_conf.h"


// TFT_eSPI tft = TFT_eSPI();

void demo_obj(){
  //1. get当前活跃屏幕
  lv_obj_t* screen = lv_scr_act();

  //2. 在当前屏幕上显示对象
  lv_obj_t* obj = lv_obj_create(screen);
  //位置、大小、颜色、样式
  lv_obj_set_pos(obj,100,50); //设置位置
  // lv_obj_set_width(obj,50);
  // lv_obj_set_height(obj,80);
  lv_obj_set_size(obj,100,150);
  lv_obj_set_style_bg_color(obj,lv_palette_main(LV_PALETTE_PINK),0);
}

void setup() {
// put your setup code here, to run once:
  //1. get当前活跃屏幕
  lv_obj_t* screen = lv_scr_act();

  lv_obj_set_style_bg_color(screen, lv_color_hex(0xFFFFFF), 0);

  // demo_gif();
  demo_obj();

}
void loop() {
// put your main code here, to run repeatedly:
}