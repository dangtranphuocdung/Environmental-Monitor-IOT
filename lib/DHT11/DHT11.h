#ifndef DHT_SENSOR_H // 1. Chống nạp chồng
#define DHT_SENSOR_H

#include <Arduino.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>

class DHTSensor {
    private: 
        int _DHTpin;
        int _DHTtype;
        DHT _dht;

    public:
        DHTSensor(int pin, int type);
        void init();
        float readTemp();
        float readHumid();
        

};
#endif
