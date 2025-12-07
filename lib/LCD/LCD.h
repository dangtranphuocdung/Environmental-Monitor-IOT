#ifndef LCD_H
#define LCD_H

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

class LcdDisplay{
    private:
        LiquidCrystal_I2C _lcd;
    public:
        LcdDisplay(uint8_t address, uint8_t characters, uint8_t column);
        void start();
        void display(float temp, float humid);
};



#endif