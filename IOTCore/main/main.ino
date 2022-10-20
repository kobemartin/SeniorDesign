#include "secrets.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"
#include <LiquidCrystal_I2C.h>
#include <Stepper.h>
#include <HTTPClient.h>


// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

// set LCD address, number of columns and rows
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

// Buzzer variables
int freq = 2000;
int channel = 0;
int resolution = 8;




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
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    lcd.print(".");
    Serial.print(".");
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
  Serial.print("incoming: ");
  Serial.println(topic);
  
  

  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);
  // Serial.println("doc: ");
  // Serial.println(doc)
  const char* message = doc["message"];

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
  return;
}


String httpGETRequest() {
  HTTPClient http;

  // Your IP address with path or Domain name with URL path 
  http.begin("https://ggst1z4j2i.execute-api.us-east-1.amazonaws.com/prod/esp32?operation=getEvents");

  // Send HTTP POST request
  int httpResponseCode = http.GET();

  String payload = "{}"; 

  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
    Serial.println(payload);
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

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



void loop() {
  client.loop();
  Serial.print("Main loop");
  delay(1000);
}