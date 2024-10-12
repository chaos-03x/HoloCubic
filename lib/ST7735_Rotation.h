
// This is the command sequence that rotates the ST7735 driver coordinate frame

  rotation = m % 5; // Limit the range of values to 0-3

  writecommand(TFT_MADCTL);
  switch (rotation) {
    case 0:
     if (tabcolor == INITR_BLACKTAB) {
       writedata(TFT_MAD_MX | TFT_MAD_MY | TFT_MAD_COLOR_ORDER);
     } else if(tabcolor == INITR_GREENTAB2) {
       writedata(TFT_MAD_MX | TFT_MAD_MY | TFT_MAD_COLOR_ORDER);
       colstart = 2;
       rowstart = 1;
     } else if(tabcolor == INITR_GREENTAB3) {
       writedata(TFT_MAD_MX | TFT_MAD_MY | TFT_MAD_COLOR_ORDER);
       colstart = 2;
       rowstart = 3;
     } else if(tabcolor == INITR_GREENTAB128) {
       writedata(TFT_MAD_MX | TFT_MAD_MY | TFT_MAD_MH | TFT_MAD_COLOR_ORDER);
       colstart = 0;
       rowstart = 32;
     } else if(tabcolor == INITR_GREENTAB160x80) {
       writedata(TFT_MAD_MX | TFT_MAD_MY | TFT_MAD_MH | TFT_MAD_COLOR_ORDER);
       colstart = 26;
       rowstart = 1;
     } else if(tabcolor == INITR_REDTAB160x80) {
       writedata(TFT_MAD_MX | TFT_MAD_MY | TFT_MAD_MH | TFT_MAD_COLOR_ORDER);
       colstart = 24;
       rowstart = 0;
     } else if(tabcolor == INITB) {
       writedata(TFT_MAD_MX | TFT_MAD_COLOR_ORDER);
     } else {
       writedata(TFT_MAD_MX | TFT_MAD_MY | TFT_MAD_COLOR_ORDER);
     }
      _width  = _init_width;
      _height = _init_height;
      break;
    case 1:
     if (tabcolor == INITR_BLACKTAB) {
       writedata(TFT_MAD_MY | TFT_MAD_MV | TFT_MAD_COLOR_ORDER);
     } else if(tabcolor == INITR_GREENTAB2) {
       writedata(TFT_MAD_MY | TFT_MAD_MV | TFT_MAD_COLOR_ORDER);
       colstart = 1;
       rowstart = 2;
     } else if(tabcolor == INITR_GREENTAB3) {
       writedata(TFT_MAD_MY | TFT_MAD_MV | TFT_MAD_COLOR_ORDER);
       colstart = 3;
       rowstart = 2;
     } else if(tabcolor == INITR_GREENTAB128) {
       writedata(TFT_MAD_MV | TFT_MAD_MY | TFT_MAD_COLOR_ORDER);
       colstart = 32;
       rowstart = 0;
     } else if(tabcolor == INITR_GREENTAB160x80) {
       writedata(TFT_MAD_MV | TFT_MAD_MY | TFT_MAD_COLOR_ORDER);
       colstart = 1;
       rowstart = 26;
     } else if(tabcolor == INITR_REDTAB160x80) {
       writedata(TFT_MAD_MV | TFT_MAD_MY | TFT_MAD_COLOR_ORDER);
       colstart = 0;
       rowstart = 24;
     } else if(tabcolor == INITB) {
       writedata(TFT_MAD_MV | TFT_MAD_MX | TFT_MAD_MY | TFT_MAD_COLOR_ORDER);
     } else {
       writedata(TFT_MAD_MY | TFT_MAD_MV | TFT_MAD_COLOR_ORDER);
     }
      _width  = _init_height;
      _height = _init_width;
      break;
    case 2:
     if (tabcolor == INITR_BLACKTAB) {
       writedata(TFT_MAD_COLOR_ORDER);
     } else if(tabcolor == INITR_GREENTAB2) {
       writedata(TFT_MAD_COLOR_ORDER);
       colstart = 2;
       rowstart = 1;
     } else if(tabcolor == INITR_GREENTAB3) {
       writedata(TFT_MAD_COLOR_ORDER);
       colstart = 2;
       rowstart = 1;
     } else if(tabcolor == INITR_GREENTAB128) {
       writedata(TFT_MAD_COLOR_ORDER);
       colstart = 0;
       rowstart = 0;
     } else if(tabcolor == INITR_GREENTAB160x80) {
       writedata(TFT_MAD_COLOR_ORDER);
       colstart = 26;
       rowstart = 1;
     } else if(tabcolor == INITR_REDTAB160x80) {
       writedata(TFT_MAD_COLOR_ORDER);
       colstart = 24;
       rowstart = 0;
     } else if(tabcolor == INITB) {
       writedata(TFT_MAD_MY | TFT_MAD_COLOR_ORDER);
     } else {
       writedata(TFT_MAD_COLOR_ORDER);
     }
      _width  = _init_width;
      _height = _init_height;
      break;
    case 3:
     if (tabcolor == INITR_BLACKTAB) {
       writedata(TFT_MAD_MX | TFT_MAD_MV | TFT_MAD_COLOR_ORDER);
     } else if(tabcolor == INITR_GREENTAB2) {
       writedata(TFT_MAD_MX | TFT_MAD_MV | TFT_MAD_COLOR_ORDER);
       colstart = 1;
       rowstart = 2;
     } else if(tabcolor == INITR_GREENTAB3) {
       writedata(TFT_MAD_MX | TFT_MAD_MV | TFT_MAD_COLOR_ORDER);
       colstart = 1;
       rowstart = 2;
     } else if(tabcolor == INITR_GREENTAB128) {
       writedata(TFT_MAD_MX | TFT_MAD_MV | TFT_MAD_COLOR_ORDER);
       colstart = 0;
       rowstart = 0;
     } else if(tabcolor == INITR_GREENTAB160x80) {
       writedata(TFT_MAD_MX | TFT_MAD_MV | TFT_MAD_COLOR_ORDER);
       colstart = 1;
       rowstart = 26;
     } else if(tabcolor == INITR_REDTAB160x80) {
       writedata(TFT_MAD_MX | TFT_MAD_MV | TFT_MAD_COLOR_ORDER);
       colstart = 0;
       rowstart = 24;
     } else if(tabcolor == INITB) {
       writedata(TFT_MAD_MV | TFT_MAD_COLOR_ORDER);
     } else {
       writedata(TFT_MAD_MX | TFT_MAD_MV | TFT_MAD_COLOR_ORDER);
     }
      _width  = _init_height;
      _height = _init_width;
      break;

      case 4:
        
        // writedata(0x39 | TFT_MAD_MV); // 设置镜像旋转(再右旋90°)
        // writedata(0x39 | TFT_MAD_MV | TFT_MAD_MY); // 没变
        // writedata(0x39 | TFT_MAD_ML);  //一样
        // writedata(0x39 | TFT_MAD_ML);  //一样
        // writedata(TFT_MAD_MV | TFT_MAD_MY | TFT_MAD_COLOR_ORDER);
        // writedata(0x49);
        // writedata(ST7735_MADCTL | TFT_MAD_MV | TFT_MAD_MY); // 行列交换并镜像上下
        // writedata(0x59);
        // writedata(ST7735_MADCTL); 
        // writedata(TFT_MAD_MV | TFT_MAD_MY); // 行列交换并上下镜像
        // writedata(0x39 | TFT_MAD_MV);
        // writedata(0x79);
        // writedata(0x39);
        // writedata(TFT_MAD_MX | TFT_MAD_MV | TFT_MAD_COLOR_ORDER);  // 顺时针90度并水平镜像
        // writedata(0x39);
        // writedata(TFT_MAD_MY | TFT_MAD_MV);
        // writedata(0x39);
        // writedata(TFT_MAD_MX);  // 水平镜像
        writedata(TFT_MAD_MY | TFT_MAD_COLOR_ORDER);  // 垂直镜像


        _width = _init_width;
        _height = _init_height;
      break;
  }
