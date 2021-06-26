#include<SPI.h>
#include <ThingerESP32.h>
#define DEBUG
#define USERNAME "Thomas_tt"
#define DEVICE_ID "ESP32"
#define DEVICE_CREDENTIAL "fa25y0%m@yzT1u78"
#define SSID "MyAltice e37ddd"
#define SSID_PASSWORD "52-emerald-4063"

#include "DHT.h"
#define DHTTYPE DHT21
#define DHTPIN 4   

ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);
DHT dht(DHTPIN, DHTTYPE);

int led = 2;       // GPIO02 - D4 

void setup() {
  Serial.begin(115200);
  dht.begin();
 // Initialization of the WiFi connection with THINGER.IO
   thing.add_wifi(SSID, SSID_PASSWORD);

 // Initialization of the LED
   pinMode(led, OUTPUT);

 // Resource for changing LED status from THINGER.IO
 /*thing["LED"] << [](pson& in){
    if(in.is_empty()){
      in = (bool) digitalRead(led);
      }
    else{
      digitalWrite(led, in ? HIGH : LOW);
      }
    };*/
   thing["DHT21"] >> [](pson& out){
      out["celsius"] = float(dht.readTemperature());
      out["humidity"] = float(dht.readHumidity());
   };
  }
 
void loop() {
  thing.handle();
  }
