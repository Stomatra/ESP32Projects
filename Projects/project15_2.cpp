#include <Arduino.h>
#include <esp32-hal-adc.h>

#define LED_PIN 13
#define POT 26
#define CHANNEL 0
#define RESOLUTION 12
#define FREQ 1000

int lastMillis = 0;
int lastMillis2 = 0;

void setup() {
    Serial.begin(9600);
    Serial.println("Hello, Arduino!");
    analogReadResolution(RESOLUTION); // Set ADC resolution to 12 bits
    analogSetAttenuation(ADC_11db); // Set attenuation for full-scale voltage
    ledcSetup(CHANNEL, FREQ, RESOLUTION); // Setup PWM on channel 0, 1kHz, 12-bit resolution
    ledcAttachPin(LED_PIN, CHANNEL); // Attach LED_PIN to PWM channel
}

void loop() {
    int potValue = analogRead(POT);
    if (millis()-lastMillis>50)
    {
        ledcWrite(CHANNEL, potValue); // Write PWM value based on potentiometer reading
        lastMillis=millis();
    }
    
    ledcWrite(CHANNEL, potValue); // Write PWM value based on potentiometer reading
    if(millis() - lastMillis2 > 1000){
      Serial.print("Potentiometer Value: ");
      Serial.println(potValue);
      lastMillis2 = millis();
    }
}