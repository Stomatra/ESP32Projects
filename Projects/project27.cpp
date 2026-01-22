#include <Arduino.h>

#define RELAY_PIN 13

hw_timer_t * timer = NULL;

void timerInterrupt(){
  digitalWrite(RELAY_PIN,!digitalRead(RELAY_PIN));
  Serial.println("Timer Interrupt Triggered");
}

void setup() {
  Serial.begin(9600);
  pinMode(RELAY_PIN, OUTPUT);

  //使用定时器中断
  //初始化定时器
  timer=timerBegin(0, 80, true); //定时器0，预分频80，计数方向上升

  timerAttachInterrupt(timer,timerInterrupt,true);
  timerAlarmWrite(timer,500*1000,true); //自动重载

  timerAlarmEnable(timer); //使能定时器中断
}

void loop() {

}