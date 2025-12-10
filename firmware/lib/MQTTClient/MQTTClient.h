#ifndef MQTTCLIENT_H
#define MQTTCLIEN_H

#include <ESP32MQTTClient.h>
#include <functional>
#include <Arduino.h>

class MqttClass {
    private:
        char* _mqtt_server;
        char* _mqtt_topic_command;
        int _port;
        ESP32MQTTClient _mqttClient;
        using UserCallback = std::function<void(String, String)>;
        UserCallback _saveCallback = nullptr;
        using ConnectCallback = std::function<void()>;
        ConnectCallback _connectCallback = nullptr;
    public:
        MqttClass();
        void begin(const char* server, int port);
        void setOnMessage(UserCallback _saveCallback);
        void setOnConnect(ConnectCallback connectInput);
        void subscribe(String topic);
        void internalOnconnect();

};


#endif