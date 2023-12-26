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

BlynkTimer timer;
MKRIoTCarrier carrier;
// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0) {
  // Set incoming value from pin V0 to a variable
  int lightsOn = param.asInt();
  if (lightsOn) {
    carrier.display.fillScreen(0xF800);
  } else {
    carrier.display.fillScreen(0x07E0);
  }
}

// This function sends temperature every second to Virtual Pin 1.
void writeTemperature() {
  // Don't send more that 10 values per second.
  float temperature = carrier.Env.readTemperature();
  Blynk.virtualWrite(V1, temperature);
}


void setup() {
  // Debug console
  Serial.begin(9600);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Setup a function to be called every second
  timer.setInterval(2000L, writeTemperature);
  //carrier.withCase();                         //Remove this if your arduino is NOT in it's case/housing
  //carrier.Buttons.updateConfig(100, TOUCH0);  //Remove this if your arduino is NOT in it's case/housing
  carrier.begin();
  carrier.display.fillScreen(0x07E0);

  while (!Serial); // Wait for Serial Monitor to open

  if (!APDS.begin()) {
    Serial.println("Error initializing APDS-9960 sensor.");
    while (true); // Stop forever
  }

  Serial.println("Sensing Gestures...");
}

void loop() {

  Blynk.run();
  timer.run();
  gesture();
  
}

void gesture() {

   // Check if a gesture reading is available
  if (APDS.gestureAvailable()) {
    int gesture = APDS.readGesture();
    switch (gesture) {
      case GESTURE_UP:
        Serial.println("Detected UP gesture");
        count++;
        Serial.println(count);
        break;

      case GESTURE_DOWN:
        Serial.println("Detected DOWN gesture");
        count--;
        break;

      case GESTURE_LEFT:
        Serial.println("Detected LEFT gesture");
        break;

      case GESTURE_RIGHT:
        Serial.println("Detected RIGHT gesture");
        break;

      default:
        // Ignore
        break;
    }
  }

  //Blynk.virtualWrite(V2, count); 
}