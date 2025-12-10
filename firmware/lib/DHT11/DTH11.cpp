#include "DHT11.h"

//Constructor: initialize the pin and call object
DHTSensor::DHTSensor(int pin, int type): _dht(pin,type){
    _DHTpin = pin;
    _DHTtype = type;
}

//Begin
void DHTSensor::init(){
    _dht.begin();
}
//Read temperature and return a float value
float DHTSensor::readTemp(){
    float temp = _dht.readTemperature();
    return temp;
}
//Read humidity and return a float value
float DHTSensor::readHumid(){
    float humid = _dht.readHumidity();
    return humid;
}

