<p align="center">
<h1 style="font-size: 36px; margin-bottom: 20px;">Automated Chicken Coop</h1>
<img src="/images/coop.png" alt="Coop Image" width="75%"/>
</p>


<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li><a href="#about-the-project">About The Project</a></li>
    <li><a href="#demonstration">Demonstration</a></li>
    <li><a href="#installation">Installation</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>

<!-- ABOUT THE PROJECT -->
# About The Project

**Assignment Overview**
* This project is being developed as part of a college IoT assignment.
* The requirement is to showcase a practical application using networking/IoT standards & protocols.

**Hardware Used**
* Arduino MKR WiFi 1010 microcontroler.
* Arduino MKR Carrier.
* APDS9960 gesture sensor (Built in to Carrier).
* An LDR for light level monitoring.

**Software & Platforms**
* Arduino IDE - For coding, compiling and uploading the programme to the Arduino MKR WiFi 1010.
* Blynk IoT Platform - Integrated with the Arduino code to enable real-time monitoring and control via the Blynk mobile app.

<p align="center">
<img src="/images/arduino-logo.png" alt="arduino" width="200" style="margin: 30px;"/>    <img src="/images/blynk-logo.png" alt="blynk" width="200" style="margin: 30px;"/>
</p>

**Primary Objectives**
* To solve a real world problem of forgetting to close the gate of the chicken run at night which often resulted in the dissapearing of hens.
* This will be achieved by monitoring and controlling the chicken coop environment using sensors and IoT principles.
* Gesture detection, light level monitoring, and manual control via a mobile app will be utilized.

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- Deployment -->
## Demonstration

* See a live demo of the project here:

  (PLACEHOLDER DEMO TO FOLLOW) https://youtu.be/smOGihCr3OI

**Flow Chart:**<br>

<p align="center">
<img src="/graphic.png" alt="Graphic" width="75%"/>
<p>

**Functionality Explained:**
* The gesture sensor on the Arduino Carrier kit is used to count chickens walking in and out of the chicken run.
* Each time movement is detected from the left, the 'count' variable is incremented.
* Each time movement is detected from the right, the 'count' variable is decremented.
* Once the count reaches 5, an event is triggered and a notification is sent to the owner's smart phone where they then have the option to close the gate by pressing a button on the Blynk app.
* When the button is pressed, a relay on the Arduino Carrier is switched on for 5 seconds, simulating a motor closing the gate.
* An LDR sensor connected to A0 on the Arduino, is used to monitor the light level.
* When the light level drops, an event is triggered and a notification is sent to the owner, where they can choose to close the gate.

<p align="right">(<a href="#readme-top">back to top</a>)</p>


### Installation

* Download the project here: 

   https://github.com/SeanKervick/Automated-Chicken-Coop.git

* Arduino IDE:

  https://www.arduino.cc/en/software

* Blynk Platform & Blynk Instructional video:

  https://blynk.io/ <br>
  https://youtu.be/smOGihCr3OI


<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- ACKNOWLEDGMENTS -->
#### Acknowledgments

Datasheets:
* Arduino MKR WiFi 1010 - https://docs.arduino.cc/resources/datasheets/ABX00023-datasheet.pdf
* Arduino Carrier - https://components101.com/sites/default/files/component_datasheet/LDR%20Datasheet.pdf
* LDR - https://components101.com/sites/default/files/component_datasheet/LDR%20Datasheet.pdf

Learning Resources:
* https://docs.arduino.cc/tutorials/mkr-iot-carrier-rev2/cheat-sheet
* https://docs.arduino.cc/built-in-examples/

<p align="right">(<a href="#readme-top">back to top</a>)</p>