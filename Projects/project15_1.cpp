#include <Arduino.h>

#define LED_PIN 13
#define POT 26

int potValue = 0;
int ledValue = 0;

int lastMillis = 0;
int lastMillis2 = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Hello, Arduino!");
  pinMode(POT, INPUT);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
    potValue = analogRead(POT);
    ledValue = map(potValue, 0, 4095, 0, 255);
    if(millis() - lastMillis > 50){
      analogWrite(LED_PIN, ledValue);
      lastMillis = millis();
    }
    if(millis() - lastMillis2 > 1000){
      Serial.print("Potentiometer Value: ");
      Serial.print(potValue);
      Serial.print("  LED Brightness: ");
      Serial.println(ledValue);
      lastMillis2 = millis();
    }
}