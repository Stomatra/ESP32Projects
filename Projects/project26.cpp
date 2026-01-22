#include<Arduino.h>
#include<WiFi.h>
#include<HTTPClient.h>
#include<ArduinoJson.h>

#define LED 2
#define LED2 4

const char* ssid     = "Redmi K70 Pro";
const char* password = "00000000";

HTTPClient http;

// 以 OpenWeather 为例（你也可以换成别的平台）
const char* API_HOST = "api.seniverse.com";
// 示例：/data/2.5/weather?q=Beijing&appid=你的key&units=metric&lang=zh_cn
String API_PATH = "/v3/weather/now.json?key=ST02OB9sce7aUP61a&location=siping&language=zh-Hans&unit=c";

void setup(){
  Serial.begin(9600);
  Serial.println("Connecting to WiFi");
  WiFi.begin(ssid, password);
  pinMode(LED,OUTPUT);
  pinMode(LED2,OUTPUT);
  while(WiFi.status()!=WL_CONNECTED){
    digitalWrite(LED,HIGH);
    delay(300);
    digitalWrite(LED,LOW);
    delay(200);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  digitalWrite(LED,HIGH);

  String payload="";
  String url = String("http://") + API_HOST + API_PATH;
  Serial.println("Requesting URL: "+url);
  http.begin(url);
  int httpCode = http.GET();
  if(httpCode>0){
    payload = http.getString();
    Serial.println("HTTP Response code: "+String(httpCode));
    Serial.println("Response payload: ");
    Serial.println(payload);
  }else{
    Serial.println("HTTP GET failed, error: "+String(httpCode));
  }

  //从给定的JSON字符串解析数据
  // JSON_OBJECT_SIZE is deprecated; use a fixed-capacity DynamicJsonDocument (adjust size if needed)
  DynamicJsonDocument doc(2048);
  DeserializationError error = deserializeJson(doc, payload);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  const char* city = doc["results"][0]["location"]["name"]; // "siping"
  const char* weather = doc["results"][0]["now"]["text"];   // "多云"
  const char* temperature = doc["results"][0]["now"]["temperature"]; // "14"
  Serial.println("City: "+String(city));
  Serial.println("Weather: "+String(weather));
  Serial.println("Temperature: "+String(temperature)+" C");

  digitalWrite(LED2,HIGH);
  http.end();
}

void loop(){

}