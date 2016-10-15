#ifndef COLORGEM_H
#define COLORGEM_H

#include "Arduino.h"

#define COLORGEM_RED 1000
#define COLORGEM_GREEN 2000

class ColorGem
{
    public:
    ColorGem(uint8_t analog_pin,uint8_t digital_pin);
    void readGem();
	uint8_t getRed(){return _red;}
	uint8_t getGreen(){return _green;}
	uint8_t getBlue(){return _blue;}
    uint16_t getValue(); // leaving this public for debugging purposes
    
    private:
	uint8_t _red;
	uint8_t	_green;
	uint8_t _blue;
    uint8_t _analog_pin;
	uint8_t _digital_pin;
};
#endif


