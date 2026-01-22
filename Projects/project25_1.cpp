#include <Arduino.h>
#include <WiFi.h>

#define LED 2

const char* ssid     = "Redmi K70 Pro";
const char* password = "00000000";

void setup(){
  Serial.begin(9600);
  Serial.println("Connecting to WiFi..");
  WiFi.begin(ssid, password);
  pinMode(LED,OUTPUT);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    digitalWrite(LED,HIGH);
    delay(200);
    digitalWrite(LED,LOW);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  digitalWrite(LED,HIGH);
  delay(3000);
  digitalWrite(LED,LOW);
}

void loop(){

}