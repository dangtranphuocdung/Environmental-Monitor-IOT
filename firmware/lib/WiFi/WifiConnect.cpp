#include "WifiConnect.h"

void Wifi::connect(String wifi_ssid, String wifi_password){
    WiFi.begin(wifi_ssid, wifi_password);
    Serial.println("Connecting to Wifi ");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.println("Wifi is not connected. ");
        delay(2000);
    }
    Serial.println();
    Serial.print("Connect to wifi: " + String(wifi_ssid));
    Serial.println("Local IP adress: ");
    Serial.println(WiFi.localIP());
}