#ifndef LED_H
#define LED_H

#include <Arduino.h>

class LedClass {
    private:
        uint8_t _freq;
        uint8_t _resolution;
        uint8_t _channel;
        
    public:
        LedClass(uint8_t pin, uint8_t channel, uint8_t freq, uint8_t reso);
        void controlLed(uint32_t pwmValue);
        void start(uint8_t pin);
};


#endif