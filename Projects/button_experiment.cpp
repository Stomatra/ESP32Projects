#include <Arduino.h>

#define LED_PIN 2
#define BUTTON_PIN 14

int led_logic=LOW;

void setup(){
    pinMode(LED_PIN,OUTPUT);
    pinMode(BUTTON_PIN,INPUT_PULLDOWN);
}

void loop(){
    if(digitalRead(BUTTON_PIN)==HIGH){
        delay(10); //消抖
        if(digitalRead(BUTTON_PIN)==HIGH){
            while(digitalRead(BUTTON_PIN)==HIGH); //等待按键释放
            led_logic= !led_logic;
            digitalWrite(LED_PIN,led_logic);
        }
    }
}