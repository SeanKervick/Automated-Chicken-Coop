#define BLYNK_TEMPLATE_ID "Insert_Here"
#define BLYNK_TEMPLATE_NAME "Insert_Here"
#define BLYNK_AUTH_TOKEN "Insert_Here"

#include <SPI.h>
#include <WiFi.h>
#include <BlynkSimpleWifi.h>
#include <Arduino_MKRIoTCarrier.h>
#include <Arduino_APDS9960.h>
#include "config.h"

int count;
int value = analogRead(A0);


BlynkTimer timer;
MKRIoTCarrier carrier;

void setup() {

  //Debug console
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  //Setup a function to be called every second
  timer.setInterval(2000L, writeChickenCount);
  timer.setInterval(2000L, writeLightLevel);
  carrier.begin();
  carrier.display.fillScreen(0x07E0);

  //Wait for Serial Monitor to open
  while (!Serial);
  //Initialize gesture sensor
  if (!APDS.begin()) {
    Serial.println("Error initializing APDS-9960 sensor.");
    //Stop forever
    while (true);  
  }
  Serial.println("Counting Chickens...");
}

void loop() {

  Blynk.run();
  timer.run();
  gesture();
  
}

//This function sends chicken count every second to Virtual Pin 1.
void writeChickenCount() {
  Blynk.virtualWrite(V1, count);
}

//This function sends the light level from LDR connected to A0 every second to Virtual Pin 2.
void writeLightLevel() {
  float lightLevel = analogRead(A0);
  Blynk.virtualWrite(V2, lightLevel);
}

void gesture() {

  //Check if a gesture reading is available
  if (APDS.gestureAvailable()) {
    int gesture = APDS.readGesture();
    switch (gesture) {

      case GESTURE_LEFT:
        Serial.println("Detected LEFT gesture");
        count++;
        Serial.println(count);
        break;

      case GESTURE_RIGHT:
        Serial.println("Detected RIGHT gesture");
        count--;
        Serial.println(count);
        break;

      default:
        //Ignore
        break;
    }
  }
}
