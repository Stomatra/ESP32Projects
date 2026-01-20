#include<Arduino.h>
#include<U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(
  U8G2_R0,     // 屏幕旋转方向
  /* clock=*/ 22,   // SCL
  /* data=*/  21,   // SDA
  /* reset=*/ U8X8_PIN_NONE
);

int progress = 0;

void setup() {
  // put your setup code here, to run once:
  u8g2.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  u8g2.firstPage();
  do {
    u8g2.drawFrame(0,30,128,20);
    u8g2.drawBox(5,35,progress,10);
  }while(u8g2.nextPage());

  if(++progress > 118){
    progress = 0;
  }
}