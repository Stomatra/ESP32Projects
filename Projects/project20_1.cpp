#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <U8g2lib.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  OLED_RESET
);

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(
  U8G2_R0,     // 屏幕旋转方向
  /* clock=*/ 22,   // SCL
  /* data=*/  21,   // SDA
  /* reset=*/ U8X8_PIN_NONE
);

void setup() {
  u8g2.begin();
  u8g2.enableUTF8Print();
  u8g2.setFont(u8g2_font_wqy12_t_chinese2);
}

void loop() {
  u8g2.clearBuffer();
  u8g2.setCursor(0, 8);
  u8g2.print("Hello, 世界!");
  u8g2.sendBuffer();
  delay(2000);
}
