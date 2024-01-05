#define BLYNK_TEMPLATE_ID "Insert_Here"
#define BLYNK_TEMPLATE_NAME "Insert_Here"
#define BLYNK_AUTH_TOKEN "Insert_Here"

#include <SPI.h>
#include <WiFi.h>
#include <BlynkSimpleWifi.h>
#include <Arduino_MKRIoTCarrier.h>
#include <Arduino_APDS9960.h>
#include "SR04.h" // Add the SR04 library for the ultrasonic sensor
#include "config.h"

int count;
BlynkTimer timer;
MKRIoTCarrier carrier;

// Ultrasonic sensor
int trigPin = 9;    // Trigger
int echoPin = 10;   // Echo
SR04 sr04 = SR04(echoPin, trigPin);

void setup() {
  // Define inputs and outputs
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

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
  gesture();
  eventTrigger();
}


// This function uses an ultrasonic sensor to monitor the status of the gate and only allows events to be triggered when the gate is open. (cmValue > 22)
void eventTrigger() {
  long cmValue = sr04.Distance();
  Serial.print("distance = ");
  Serial.println(cmValue);

  // Check the condition without using a while loop
  if (cmValue > 22) {
    // Trigger event to notify owner when 5 chickens have entered the coop.
    if (count > 4) {
      Blynk.logEvent("full_coop", "There are 5 chickens in the coop. Would you like to close the gate?");
    }

    // Trigger event to notify owner when it gets dark outside.
    float lightLevel = analogRead(A0);
    if (lightLevel > 850) {
      Blynk.logEvent("light_level", "It is dark outside, would you like to close the gate?");
    }
  }
}

// This function send the data to Blynk, it is called in the setup every 2 seconds.
void sendData() {
  // Send chicken count to Virtual Pin 1
  Blynk.virtualWrite(V1, count);

  // Read and send light level data from LDR connected to A0 to Virtual Pin 2.
  float lightLevel = analogRead(A0);
  Blynk.virtualWrite(V2, lightLevel);
}

// This function is called when the state of the virtual pin V0 changes on the Blynk app.
// It is used to turn on a relay for 5 seconds when the button is pressed, simulating a motor closing the gate.
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

// This function uses the gesture sensor to count chickens walking in and out of the run.
// The carrier display flashes green or red depending on which side it detects movement from.
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