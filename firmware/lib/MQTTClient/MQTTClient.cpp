#include "MQTTClient.h"
#include <Arduino.h>

static MqttClass* instance = nullptr;

//Hàm GLOBAL bắt buộc phải có tên này để thư viện gốc gọi
void onMqttConnect(esp_mqtt_client_handle_t client){
    if (instance != nullptr){
        // Nhờ biến tĩnh gọi vào hàm bên trong class
        instance ->internalOnconnect();
    }
}

MqttClass::MqttClass(){
    // Khi khởi tạo, trỏ cầu nối vào chính mình
    instance = this;
}

void MqttClass::internalOnconnect(){
    Serial.println("Sucessfully Connected ");
    if (_connectCallback != nullptr){
        _connectCallback();
    }
}


void MqttClass::setOnMessage(UserCallback saveCallback){
    _saveCallback = saveCallback;
}

void MqttClass::setOnConnect(ConnectCallback connectInput){
    _connectCallback = connectInput;
}

void MqttClass::subscribe(String topic){ //Subscribe and get the message
    std::string topicStr = std::string(topic.c_str());
    _mqttClient.subscribe(topicStr, [this](const std::string &t, const std::string &p) {
        // Khi nhận tin, gọi hàm của Main ra xử lý

        if (_saveCallback != nullptr) {
            _saveCallback(String(t.c_str()), String(p.c_str()));
        }
    }, 0);
}

void MqttClass::begin(const char* server, int port){
    _port = port;
    //_mqtt_server = server;
    _mqttClient.setURL(&server[0], _port);
    String randomId = "ESP32_User_" + String(random(0xFFFF), HEX);
    _mqttClient.setMqttClientName(randomId.c_str());
    _mqttClient.loopStart();
}
