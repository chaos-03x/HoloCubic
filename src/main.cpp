#include <SPI.h> //导入库
#include <TFT_eSPI.h>
#include "lvgl.h"
#include "lv_conf.h"
#include "..\.pio\libdeps\esp32dev\lvgl\src\libs\gif\lv_gif.c"
#include "alarm_clock_filled.c"


// TFT_eSPI tft = TFT_eSPI();

void demo_gif(){
  //1. 声明图片
  LV_IMG_DECLARE(alarm_clock_filled);

  //2. 创建lv_gif对象
  lv_obj_t* screen = lv_scr_act();
  lv_obj_t* gif = lv_gif_create(screen);
  // lv_obj_t* gif = lv_gif_create(lv_scr_act());

  //3. 给对象设置图片
  lv_gif_set_src(gif, &alarm_clock_filled);

  //4. 样式
  lv_obj_align(gif, LV_ALIGN_CENTER, 0, -20);
  // lv_obj_set_size(gif, 200, 200);

}

void setup() {
// put your setup code here, to run once:
  //1. get当前活跃屏幕
  lv_obj_t* screen = lv_scr_act();

  lv_obj_set_style_bg_color(screen, lv_color_hex(0xFFFFFF), 0);

  demo_gif();

}
void loop() {
// put your main code here, to run repeatedly:
}