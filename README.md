# Medicine Merry Go Round 🎠

This repository contains code for the Medicine Merry Go Round Senior Design project.

The software component will consist of two major parts: ESP32, AWS
1. ESP32 Code
2. Amazon Web Services (AWS IOT Core, AWS S3, AWS Amplify, AWS Cloud Watch)

# System Activity Diagram
![alt text](https://raw.githubusercontent.com/kobemartin/SeniorDesign/master/SystemActivityDiagram.png)



## Arduino IDE Setup
The following instructions will be updated to document what needs to be done to compile the repository code on an ESP32
### Library Dependencies
- ESP32
- Arduino JSON
- PubSubClient



## Major Updates
(September 29, 2022) – Initial commit of ESP32 code for interaction with AWS IOT Core. This code allows AWS to trigger an event on the ESP32 via the MQTT protocol.

(September 20, 2022) – ESP32 code can now invoke buzzer and motor on specific events. ESP32 can get data from AWS API. Webpage updated with user input form. Sends data using to AWS API. Website also gets data from AWS API and displays results on a HTML table.