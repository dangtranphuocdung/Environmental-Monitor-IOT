#include <Arduino.h>
#include "DHT11.h"
#include "FirebaseClient.h"
#include "WifiConnect.h"
#include "Config.h"
#include <Wifi.h>
#include "LCD.h"

const int pin = 27;
float temp;
float humid;
unsigned long lastSendTime = 0;
unsigned long currentTime;
unsigned long timeInterval = 10000;

DHTSensor dht(pin, DHT11);
LcdDisplay lcd(0x27, 16, 2);
FirebaseSetting firebaseconfig(API_KEY, DATABES_URL);
Wifi wifiConnect;
void setup() {
  dht.init();
  lcd.start();
  firebaseconfig.login();
  firebaseconfig.begin();
  wifiConnect.connect(WIFI_SSID, PASSWORD);
}

void loop() {
  temp = dht.readTemp();
  humid = dht.readHumid();
  lcd.display(temp,humid);
  firebaseconfig.sendDataToFBS(temp, humid);
}
