#include "secrets.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"
#include <LiquidCrystal_I2C.h>
#include <Stepper.h>
#include <HTTPClient.h>
#include "time.h"


// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

// set LCD address, number of columns and rows
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

// Buzzer variables
int freq = 2000;
int channel = 0;
int resolution = 8;

bool lcdLock = false;



const int stepsPerRevolution = 256;  // change this to fit the number of steps per revolution

// ULN2003 Motor Driver Pins
#define IN1 19
#define IN2 18
#define IN3 5
#define IN4 17

// initialize the stepper library
Stepper myStepper(stepsPerRevolution, IN1, IN3, IN2, IN4);
// myStepper.setSpeed(4);


#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"
#define lamp 23

WiFiClientSecure net = WiFiClientSecure();
PubSubClient client(net);

void scrollText(int row, String message, int delayTime, int lcdColumns) {
  for (int i = 0; i < lcdColumns; i++) {
    message = " " + message;
  }
  message = message + " ";
  for (int pos = 0; pos < message.length(); pos++) {
    lcd.setCursor(0, row);
    lcd.print(message.substring(pos, pos + lcdColumns));
    delay(delayTime);
  }
}


void connectAWS() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println("Connecting to Wi-Fi");

  // set cursor to first column, first row
  lcd.setCursor(0, 0);
  // print message
  lcd.print("Wi-Fi Connecting");

  lcd.setCursor(0, 1);
  int dotCount = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    if (dotCount < 16)
    {
      lcd.print(".");
      Serial.print(".");
      dotCount += 1;
    } else {
      lcd.clear();
      lcd.print("Wi-Fi Connecting");
      lcd.setCursor(0, 1);
      dotCount = 0;
    }
    
  }
  lcd.clear();

  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.setServer(AWS_IOT_ENDPOINT, 8883);

  // Create a message handler
  client.setCallback(messageHandler);

  Serial.println("Connecting to AWS IOT");
  lcd.setCursor(0, 0);
  lcd.print("AWS Connecting");

  lcd.setCursor(0, 1);
  while (!client.connect(THINGNAME)) {
    delay(100);
    Serial.print(".");
    lcd.print(".");
  }
  lcd.clear();

  if (!client.connected()) {
    Serial.println("AWS IoT Timeout!");
    return;
  }

  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);

  Serial.println("AWS IoT Connected!");
  return;
}

void buzzer() {
  ledcSetup(channel, freq, resolution);
  ledcAttachPin(16, channel);
  ledcWriteTone(channel, 2000);
  
  for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle=dutyCycle+10){
  
    Serial.println(dutyCycle);
  
    ledcWrite(channel, dutyCycle);
    delay(100);
  }
  
  ledcWrite(channel, 125);
  
  for (int freq = 255; freq < 7000; freq = freq + 250){
  
     Serial.println(freq);
  
     ledcWriteTone(channel, freq);
     delay(100);
  }
  ledcDetachPin(16);

  return;
}

void dispenseEvent( ) {
  

  buzzer();
  myStepper.setSpeed(4);
  myStepper.step(stepsPerRevolution);
}


void messageHandler(char* topic, byte* payload, unsigned int length) {
  httpGETRequest();
  Serial.print("incoming: ");
  Serial.println(topic);
  
  

  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);
  // Serial.println("doc: ");
  // Serial.println(doc)
  const char* message = doc["message"];

  lcdLock = true;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(message);

  Serial.println();
  Serial.print("--------------");
  Serial.print(payload[0]);
  Serial.print("--------------");
  Serial.print(payload[1]);
  Serial.print("--------------");
  Serial.print(message);
  Serial.print("--------------");



  // for (int i = 0; i < length; i++) {
  //   Serial.print((char)payload[i]);  // Pring payload content
  // }
  // char led = (char)payload[62];  // Extracting the controlling command from the Payload to Controlling LED from AWS
  // Serial.print("Command: ");
  // Serial.println(led);

  // if (led == 49)  // 49 is the ASCI value of 1
  // {
  //   digitalWrite(lamp, HIGH);
  //   Serial.println("Lamp_State changed to HIGH");
  // } else if (led == 48)  // 48 is the ASCI value of 0
  // {
  //   digitalWrite(lamp, LOW);
  //   Serial.println("Lamp_State changed to LOW");
  // }
  Serial.print("here");
  Serial.println();
  sleep(10);
  lcdLock = false;
  return;
}

String payload = "{}"; 

struct event {
  int eventID;
  int day;
  int hour;
  int minute;
  String medication;
};

event myEvents[14];


void updateEvents(const StaticJsonDocument<2048>&  doc) {
  for (int i = 0; i < 14; i++) {
    Serial.println("hi");
    String day;
    String time;
    String medication;
    serializeJsonPretty(doc[String(i + 1)]["day"], day);
    serializeJsonPretty(doc[String(i + 1)]["time"], time);
    serializeJsonPretty(doc[String(i + 1)]["medication"], medication);
    int dayInt = -1;
    Serial.println(String(i) + " " + day);
    if (day == "\"Monday\"") {
      dayInt = 1;
    } else if (day == "\"Tuesday\"") {
      dayInt = 2;
    } else if (day == "\"Wednesday\"") {
      dayInt = 3;
    } else if (day == "\"Thursday\"") {
      dayInt = 4;
    } else if (day == "\"Friday\"") {
      dayInt = 5;
    } else if (day == "\"Saturday\"") {
      dayInt = 6;
    } else if (day == "\"Sunday\"") {
      dayInt = 7;
    }
    myEvents[i].medication = medication;
    myEvents[i].day = dayInt;
    myEvents[i].hour = time.substring(1,3).toInt();
    myEvents[i].minute = time.substring(4,6).toInt();

  }
}

void printEventObjects() {
  Serial.println("START EVENT OBJECTS:");
  for (int i; i < 14; i++) {
    Serial.println("Event: " + String(i) + " day: " + String(myEvents[i].day) + " hour: " + String(myEvents[i].hour) + " minute: " + String(myEvents[i].minute) + " medication: " + String(myEvents[i].medication));
  }
  Serial.println("END EVENT OBJECTS:");
}

String httpGETRequest() {
  HTTPClient http;

  // Your IP address with path or Domain name with URL path 
  http.begin("https://ggst1z4j2i.execute-api.us-east-1.amazonaws.com/prod/esp32?operation=getEvents");

  // Send HTTP POST request
  int httpResponseCode = http.GET();

  // String payload = "{}"; 



  if (httpResponseCode>0) {
    Serial.println("\nPAYLOAD:");
    Serial.print("\n\nHTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
    Serial.println(payload);

    Serial.println("\nJSON:");
    StaticJsonDocument<2048> doc; 
    deserializeJson(doc, payload);
    serializeJsonPretty(doc, Serial);
    updateEvents(doc);
    printEventObjects();



  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();
  Serial.println("\n\n");

  return payload;
}


void setup() {
  Serial.begin(115200);
  
  // buzzer();
  // initialize LCD
  lcd.init();
  // turn on LCD backlight
  lcd.backlight();


  connectAWS();

  httpGETRequest();
  // pinMode(lamp, OUTPUT);
  // digitalWrite(lamp, LOW);

  
}

void displayTime() {
;
}

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -5 * 3600;
const int   daylightOffset_sec = 3600;
struct tm timeinfo;
const char* day = "Unknown";

void printLocalTime()
{
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  if (!lcdLock) {
    Serial.println(&timeinfo, "PRINT1: %A, %B %d %Y %H:%M:%S");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(&timeinfo, "%B %d, %Y");
    lcd.setCursor(0, 1);
    lcd.print(&timeinfo, "%H:%M:%S");
    day = (&timeinfo, "%B %d, %Y");
    Serial.print("__________");
    Serial.print("PRINT2");
    Serial.print(timeinfo.tm_wday);
    Serial.print(day);
    Serial.print("e__________");  
  }
  
}

void checkForEvent()
{
  int day = timeinfo.tm_wday;
  int hour = timeinfo.tm_hour;
  int minute = timeinfo.tm_min;
  Serial.println("_____S-----_____");
  Serial.println("Hi: day: " + String(day) + " hour: " + String(hour) + " minute: " +  String(minute));
  Serial.println("_____-----_____");
  for (int i; i < 14; i++) {
    if (day == myEvents[i].day && hour == myEvents[i].hour && minute == myEvents[i].minute) {
      lcdLock = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Time to take:");
      lcd.setCursor(0,1);
      lcd.print(myEvents[i].medication);
      Serial.println("EVENT TRIGGERED!!!");
      dispenseEvent( );
      lcdLock = false;
    }
  }
}


void loop() {
  client.loop();
  Serial.print("Main loop");
  printLocalTime();
  checkForEvent();
  delay(1000);
  
}