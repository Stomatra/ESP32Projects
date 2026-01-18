#include <Arduino.h>
#include <esp32-hal-ledc.h>
#include <cmath>

#define FREQ 5000
#define CHANNEL 0
#define RESOLUTION 8

#define LED_PIN 12

void setup(){
    ledcSetup(CHANNEL, FREQ, RESOLUTION);
    ledcAttachPin(LED_PIN, CHANNEL);
}

void loop(){
    for(int duty = 0; duty <= 255; duty++){
        ledcWrite(CHANNEL, duty);
        delay(10);
    }
    for(int duty = 255; duty >= 0; duty--){
        ledcWrite(CHANNEL, duty);
        delay(10);
    }
}