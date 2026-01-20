#include <Arduino.h>
#include <Ticker.h>

#define LED 2
#define LED_ONCE 4

Ticker timer;
Ticker timer_once;

void toggle(int pin){
  digitalWrite(pin, !digitalRead(pin));
}

void setup(){
  pinMode(LED,OUTPUT);
  pinMode(LED_ONCE,OUTPUT);

  timer.attach(1.0, toggle, LED); // Toggle LED every 1 second
  timer_once.once(2.0, toggle, LED_ONCE); // Toggle LED_ON
}

void loop(){

}