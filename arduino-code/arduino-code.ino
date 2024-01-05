#define BLYNK_TEMPLATE_ID "Insert_Here"
#define BLYNK_TEMPLATE_NAME "Insert_Here"
#define BLYNK_AUTH_TOKEN "Insert_Here"

// Required libraries
#include <SPI.h>
#include <WiFi.h>
#include <BlynkSimpleWifi.h>
#include <Arduino_MKRIoTCarrier.h>
#include <Arduino_APDS9960.h> // Gesture sensor.
#include "config.h"

int count; // Chicken counter
int proximity; // Proximity sensor (Gate status)
int isGateClosed = 0; // 0 for open 1 for closed.

BlynkTimer timer;
MKRIoTCarrier carrier;


void setup() {
  // Debug console
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  // Setup a function to be called every second
  timer.setInterval(2000L, sendData);
  carrier.begin();
  // Wait for Serial Monitor to open
  while (!Serial);
  // Initialize gesture sensor
  if (!APDS.begin()) {
    Serial.println("Error initializing APDS-9960 sensor.");
    // Stop forever
    while (true);
  }
  Serial.println("Counting Chickens...");
}

void loop() {
  Blynk.run();
  timer.run();
  countChickens();
  eventTrigger();
}

// This function uses the gesture sensor to count chickens walking in and out of the run.
// The carrier display flashes green or red depending on which side it detects movement from.
void countChickens() {
  // Check if a gesture reading is available
  if (APDS.gestureAvailable()) {
    int gesture = APDS.readGesture();
    switch (gesture) {
      case GESTURE_LEFT:
        Serial.println("Detected LEFT gesture");
        count++;
        Serial.print("Chicken count = ");
        Serial.println(count);
        carrier.display.fillScreen(0x07E0);
        delay(300);
        carrier.display.fillScreen(0x0000);
        break;

      case GESTURE_RIGHT:
        Serial.println("Detected RIGHT gesture");
        count--;
        Serial.print("Chicken count = ");
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

// This function uses the proximity sensor to monitor the status of the gate and only allows events to be triggered when the gate is open. (proximity !=0)
void eventTrigger() {
  // check if a proximity reading is available
  if (APDS.proximityAvailable()) {
    // read the proximity
    int proximity = APDS.readProximity();
    delay(50);
    // print value to the Serial Monitor
    Serial.print("Proximtity = ");
    Serial.println(proximity);
    Serial.print("Gate Status = ");
    Serial.println(isGateClosed);

    // When the gate is open
    if (proximity != 0) {
      isGateClosed = 0; // Status of the gate is assigned.

      // Trigger event to notify owner when all chickens have entered the coop.
      if (count > 4) {
        Blynk.logEvent("full_coop", "There are 5 chickens in the coop. Would you like to close the gate?");
      }

      // Trigger event to notify owner when it gets dark outside but coop is not full.
      float lightLevel = analogRead(A0);
      // Check light level and chicken count.
      if (lightLevel > 850 && count < 5) {
        Blynk.logEvent("light_level", "It is dark outside, but all chickens are not accounted for!");
      }

      // Automatically close gate if all hens are in and it is dark.
      if (count > 4 && lightLevel > 850) {
        // Trigger the relay for 5 seconds, simulating a motor closing the gate.
        carrier.Relay1.open();
        delay(5000);
        carrier.Relay1.close();
    
        // Log an event indicating that the gate is being closed.
        Blynk.logEvent("auto_gate", "Closing the gate automatically.");
      }
      // When the gate is closed.
    } else {
      Serial.println("THE GATE IS CLOSED");
      delay(1000);
      isGateClosed = 1; //Status of the gate is assigned.
      Serial.print("Gate Status = ");
      Serial.println(isGateClosed);
    }
  }
}

// This function sends the data from the Arduino to Blynk, it is called every 2 seconds. The timer is set in the setup.
void sendData() {
  // Send chicken count to Virtual Pin 1
  Blynk.virtualWrite(V1, count);

  // Read and send light level data from LDR connected to A0 to Virtual Pin 2.
  float lightLevel = analogRead(A0);
  Blynk.virtualWrite(V2, lightLevel);

  // Send gate status to Virtual Pin 3
  Blynk.virtualWrite(V3, isGateClosed);
  
}

// This function is called when the state of the virtual pin V0 changes on the Blynk app.
// It is used to turn on a relay for 5 seconds when the button is pressed, simulating a motor closing the gate.
BLYNK_WRITE(V0) {
  // Set incoming value from pin V0 to a variable
  int relayOn = param.asInt();
  if (relayOn) {
    carrier.Relay1.open();
    delay(5000);
    carrier.Relay1.close();
  }
}
