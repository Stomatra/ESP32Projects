#include <Arduino.h>

#define LED 2
#define LED_ONCE 4

hw_timer_t * timer = NULL;
hw_timer_t * timer_once = NULL;

void timer_interrupt(){
  digitalWrite(LED, !digitalRead(LED));
}

void timer_once_interrupt(){
  digitalWrite(LED_ONCE, !digitalRead(LED_ONCE));
}

void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  pinMode(LED_ONCE, OUTPUT);

  timer = timerBegin(0, 80, true); // 1 microsecond per tick
  timer_once = timerBegin(1, 80, true); // 1 microsecond per tick

  timerAttachInterrupt(timer, &timer_interrupt, true);
  timerAttachInterrupt(timer_once, &timer_once_interrupt, true);

  timerAlarmWrite(timer, 1000*1000, true);
  timerAlarmWrite(timer_once, 2000*1000, false);

  timerAlarmEnable(timer);
  timerAlarmEnable(timer_once);
}

void loop() {
  // put your main code here, to run repeatedly:
}