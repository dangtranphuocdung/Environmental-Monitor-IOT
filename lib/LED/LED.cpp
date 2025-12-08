#include "LED.h"
#include <Arduino.h>

LedClass::LedClass(uint8_t pin ,uint8_t channel, uint8_t freq, uint8_t reso){
    _channel = channel;
    _freq = freq;
    _resolution = reso;
    ledcSetup(_channel, _freq, _resolution);
    ledcAttachPin(pin, _channel);
}

void LedClass::start(uint8_t pin){
    pinMode(pin, OUTPUT);
}

void LedClass::controlLed(uint32_t pwmValue){
    if (_resolution == 8){
        if (pwmValue > 255){
            pwmValue = 255;
        }
    }
    else if (_resolution == 10){
        if (pwmValue > 1023){
            pwmValue = 1023;
        }
    }
    else if (_resolution == 12){
        if (pwmValue > 4095){
            pwmValue = 4095;
        }
    }

    ledcWrite(_channel, pwmValue);
}