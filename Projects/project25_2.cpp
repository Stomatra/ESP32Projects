#include<Arduino.h>
#include<WiFi.h>

#define LED 2
#define LED2 4

const char* ssid     = "ESP32_Network";
const char* password = "00000000";

void setup(){
  Serial.begin(9600);

  //启动AP模式
  WiFi.softAP(ssid, password);
  Serial.println("Access Point Started");
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());
  pinMode(LED,OUTPUT);
  digitalWrite(LED,HIGH);

  //如果有设备连接上来，会触发下面的事件
  pinMode(LED2,OUTPUT);  
  WiFi.onEvent([](arduino_event_id_t /*event*/, arduino_event_info_t /*info*/){
    Serial.println("Device Connected");
    digitalWrite(LED2,HIGH);
  }, ARDUINO_EVENT_WIFI_AP_STACONNECTED);

  WiFi.onEvent([](arduino_event_id_t /*event*/, arduino_event_info_t /*info*/){
    Serial.println("Device Disconnected");
    digitalWrite(LED2,LOW);
  }, ARDUINO_EVENT_WIFI_AP_STADISCONNECTED);
}

void loop(){

}