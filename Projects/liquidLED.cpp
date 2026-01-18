#include <Arduino.h>

int LED_PIN[5]={13,12,14,27,26};
int num=sizeof(LED_PIN)/sizeof(LED_PIN[0]);

void setup() {
  // put your setup code here, to run once:
  //设定引脚为输出模式
  for(int i=0;i<num;i++){
    pinMode(LED_PIN[i],OUTPUT);
  }
}
void loop() {
  // put your main code here, to run repeatedly:
  //点亮LED
  for(int i=0;i<num;i++){
    digitalWrite(LED_PIN[i],HIGH);
    delay(100);
  }
  for(int i=0;i<num;i++){
    digitalWrite(LED_PIN[i],LOW);
    delay(100);
  }
}
