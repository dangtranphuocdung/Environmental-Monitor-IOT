#include "lcd.h"

LcdDisplay::LcdDisplay(uint8_t address, uint8_t characters, uint8_t column): _lcd(address, characters, column){

}

void LcdDisplay::start(){
    _lcd.init();
    _lcd.clear();
    _lcd.backlight();
};

void LcdDisplay::display(float temp, float humid){
    _lcd.setCursor(0,0); //Set cursor to character 2 on line 1
    _lcd.print("Temp is " + String(temp));
    _lcd.setCursor(0,1); //Set cursor tại character 2, line 2
    _lcd.print("Humidity is " + String(humid));
};

void LcdDisplay::isDataValid(float temp, float humid){
    if (isnan(temp) || isnan(humid)) {
        Serial.println("False to read data from sensor");
        _lcd.clear();
        _lcd.setCursor(0,0);
        _lcd.print("Sensor Error");
        return;
    }
};
