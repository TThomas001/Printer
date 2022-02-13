#include <Arduino.h>
#include<SPI.h>
#include <ThingerESP32.h>
#include <FanController.h>

/*thinger.io setting*/
#define USERNAME "Thomas_tt"
#define DEVICE_ID "ESP32"
/*#define DEVICE_CREDENTIAL "fa25y0%m@yzT1u78"
#define SSID "Home"
#define SSID_PASSWORD "Parkwest3622tamu!"
*/
#define DEVICE_CREDENTIAL "C%GRMLy7R2!-!zw7"
#define SSID "LAPTOP-IUIICA2V4647"
#define SSID_PASSWORD "G/7g5806"

// the rest of your sketch goes here

/*DHT sensor setting*/
#include "DHT.h"
#define DHTTYPE DHT21
#define DHTPIN 4   

/*Fan control setting*/
#define SENSOR_PIN 25
#define SENSOR_THRESHOLD 1000 //the threshold will defind as time interval.(Large scale led to better accuracy)
#define PWM_PIN 33

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
int Duty_cycle = 0;

/*ADXL335 acceleromenter*/
const int xInput = 32;
const int yInput = 5;
const int zInput = 27;
const int sampleSize = 100000;
int RawMin = 0;
int RawMax = 1023;

ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);
DHT dht(DHTPIN, DHTTYPE);
FanController fan(SENSOR_PIN, SENSOR_THRESHOLD, PWM_PIN);


void setup() {
  Serial.begin(115200);
  dht.begin();
  fan.begin();

 // Initialization of the WiFi connection with THINGER.IO
  thing.add_wifi(SSID, SSID_PASSWORD);

  pinMode(SENSOR_PIN, INPUT_PULLUP);// set the resister becuase the fan voltage reference outside source.

/*Fan control API */
 thing["Fan"] = [](pson& in, pson& out){
  if(in["speed"].is_empty()){
        in["speed"] = Duty_cycle;
    }
    else{
        Duty_cycle = in["speed"];
    }

   byte target = max(min(Duty_cycle, 100), 0);
   fan.setDutyCycle(target);
   unsigned int rpms=fan.getSpeed();
   out["RPM"]=rpms;
 };
/*DHT API */

   thing["DHT21"] >> [](pson& out){
      out["celsius"] = float(dht.readTemperature());
      out["humidity"] = float(dht.readHumidity());
   };
  }
 
void loop() {
  thing.handle();
  }

float Readangle(int axis) //convert the ADXL335 analogy signal to to angle
{
	long reading = 0;
	analogRead(axis);
	delay(1);
	for (int i = 0; i < sampleSize; i++)// Take samples and return the average
	{
	reading += analogRead(axis);
	}
	 int raw= reading/sampleSize;
  return map(raw, RawMin, RawMax, -3000, 3000)/1000;
}

