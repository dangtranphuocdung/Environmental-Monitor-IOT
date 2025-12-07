#include "DHT11.h"

//Constructor
DHTSensor::DHTSensor(int pin, int type): _dht(pin,type){
    _DHTpin = pin;
    _DHTtype = type;
}

//Begin
void DHTSensor::init(){
    _dht.begin();
}

float DHTSensor::readTemp(){
    float temp = _dht.readTemperature();
    return temp;
}

float DHTSensor::readHumid(){
    float humid = _dht.readHumidity();
    return humid;
}

