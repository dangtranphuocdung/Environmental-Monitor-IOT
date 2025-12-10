#include <Arduino.h>
#include "DHT11.h"
#include "FirebaseClient.h"
#include "WifiConnect.h"
#include "config.h"
#include <Wifi.h>
#include "LCD.h"
#include "LED.h"
#include "MQTTClient.h"

//Set variables
const int pin = 27;
const int ledPin = 18;
float temp;
float humid;
unsigned long lastSendTime = 0;
unsigned long currentTime;
unsigned long timeInterval = 10000;
const char* mqtt_server = "broker.hivemq.com";
const char* mqtt_topic_command = "house/livingroom/led/set";

//Call objects and config API, Database.

DHTSensor dht(pin, DHT11);
LcdDisplay lcd(0x27, 16, 2);
FirebaseSetting firebaseconfig(API_KEY, DATABES_URL);
Wifi wifiConnect;
LedClass led1(ledPin, 0, 5000, 8);
MqttClass mqttClient;

//Callback: process messsages from broker and responde
void onMessage(String topic, String message) {
  Serial.print("Message from topic: ");
  Serial.print(topic);
  Serial.print("Context: ");
  Serial.print(message);

  if (message == "ON") {
    led1.controlLed(255);
  }
  else if (message == "OFF") {
    led1.controlLed(0);
  }
};
//Callback: Subscribe to topic after sucessfully set up client and wait for message from that
void onConnection(){
  Serial.println("Connected to MQTT");
  mqttClient.subscribe(&mqtt_topic_command[0]);
  Serial.println("Subscribed to toppic");
  Serial.println(mqtt_topic_command);
}


void setup() {
  Serial.begin(9600);
  wifiConnect.connect(WIFI_SSID, PASSWORD); //Connect wifi
  firebaseconfig.login(); //Connect firebase
  firebaseconfig.begin();
  dht.init(); //Connect hardware
  lcd.start();
  led1.start(ledPin);
  mqttClient.setOnConnect(onConnection);//Subscribe to 
  mqttClient.setOnMessage(onMessage);
  mqttClient.begin(&mqtt_server[0], 1883);
}

void loop() {
  currentTime = millis();
  if (currentTime - lastSendTime >= timeInterval){
    lastSendTime = currentTime;
    temp = dht.readTemp();
    humid = dht.readHumid();
    lcd.isDataValid(temp, humid);
    lcd.display(temp,humid);
    firebaseconfig.sendDataToFBS(temp, humid);
  }
}
