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

void setup() {
  //Debug console
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  //Setup a function to be called every second
  timer.setInterval(2000L, sendData);
  carrier.begin();

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

void sendData() {
  // Send chicken count to Virtual Pin 1
  Blynk.virtualWrite(V1, count);
  // Read and send light level data from LDR connected to A0 to Virtual Pin 2.
  float lightLevel = analogRead(A0);
  Blynk.virtualWrite(V2, lightLevel);

  // Trigger event to notify owner when 5 chickens have entereed the coop.
  if (count > 4) {
  Blynk.logEvent("full_coop", "There are 5 chickens in the coop. Would you like to close the gate?");
  }

  // Trigger event to notify owner when it gets dark outside.
  if (lightLevel > 850) {
  Blynk.logEvent("light_level", "It is dark outside, would you like to close the gate?");
  }
}

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0) {
  // Set incoming value from pin V0 to a variable
  int relayOn = param.asInt();
  // Relay turns on for 5 seconds when the switch is pressed, simulating a motor closing the gate.
  if (relayOn) {
    carrier.Relay1.open();
    delay(5000);
    carrier.Relay1.close();
  } 
}

void gesture() {
  // Check if a gesture reading is available
  if (APDS.gestureAvailable()) {
    int gesture = APDS.readGesture();
    switch (gesture) {

      case GESTURE_LEFT:
        Serial.println("Detected LEFT gesture");
        count++;
        Serial.println(count);
        carrier.display.fillScreen(0x07E0);
        delay(300);
        carrier.display.fillScreen(0x0000);
        break;

      case GESTURE_RIGHT:
        Serial.println("Detected RIGHT gesture");
        count--;
        Serial.println(count);
        carrier.display.fillScreen(0xF800);
        delay(300);
        carrier.display.fillScreen(0x0000);
        break;

      default:
        // Ignore
        break;
    }
  }
}
