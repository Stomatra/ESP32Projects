#include<Arduino.h>

#define BUTTON 14
#define LED 2

volatile bool flag = false;

void handle_interrupt(){
  flag = true;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(BUTTON, INPUT_PULLDOWN);
  pinMode(LED, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(BUTTON),handle_interrupt,FALLING);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(flag){
    digitalWrite(LED,HIGH);
    Serial.println("Interrupt Triggered");
    delay(2000);
    digitalWrite(LED,LOW);
    flag = false;
  }
}