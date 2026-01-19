#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

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

int i=0;

void setup() {
  Wire.begin(21, 22);   // SDA, SCL
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);

  display.clearDisplay();
  display.drawRoundRect(0,16,128,16,4,SSD1306_WHITE);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("SSD1306 I2C Progress Bar");
  display.display();
}

void loop() {
  if(i<128){
    i++;
    display.fillRoundRect(1,17,i,14,4,SSD1306_WHITE);
    display.display();
    delay(100);
  }
  else{
    while(1);
  }
}
