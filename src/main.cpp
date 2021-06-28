
#include <Arduino.h>
#include<SPI.h>
#include <ThingerESP32.h>
#include <FanController.h>

/*thinger.io setting*/
#define USERNAME "Thomas_tt"
#define DEVICE_ID "Fan"
#define DEVICE_CREDENTIAL "C%GRMLy7R2!-!zw7"
#define SSID "LAPTOP-IUIICA2V 4647"
#define SSID_PASSWORD "G/7g5806"


/*DHT sensor setting*/
#include "DHT.h"
#define DHTTYPE DHT21
#define DHTPIN 4   

/*Fan control setting*/
#define SENSOR_PIN 12
#define SENSOR_THRESHOLD 1000 //the threshold will defind as time interval.(Large scale led to better accuracy)
#define PWM_PIN 5
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);
DHT dht(DHTPIN, DHTTYPE);
FanController fan(SENSOR_PIN, SENSOR_THRESHOLD, PWM_PIN);
#define DEBUG
void setup() {
  Serial.begin(115200);
  dht.begin();
  fan.begin();
 // Initialization of the WiFi connection with THINGER.IO
  thing.add_wifi(SSID, SSID_PASSWORD);

 thing["Fan"] << [](pson& in){
   int Duty_cycle = in["Speed"];
   byte target = max(min(Duty_cycle, 100), 0);
   fan.setDutyCycle(target);
   };

   /* if(in.is_empty()){
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
