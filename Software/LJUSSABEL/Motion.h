#ifndef MOTION_h
#define MOTION_h

#include "Arduino.h"

// Min is theoretically 0 (0*0+0*0+0*0)
// Max is theoretically 27 (3*3+3*3+3*3) for the ADXL335
#define SWING_THRESHOLD_MAX 27
#define SWING_THRESHOLD_MIN 1

#define SWING_SAMPLE_TIME 200

class Motion
{
	public:
	// Use output pins as vcc and gnd
	Motion(uint8_t pin_x, uint8_t pin_y, uint8_t pin_z, float aref, uint8_t pin_vcc, uint8_t pin_gnd);
	// Optional constructor if you wire GND and VCC separately.
	//Motion(uint8_t pin_x, uint8_t pin_y, uint8_t pin_z, uint8_t aref);
	
	void update();
	bool increaseSwingThreshold(); //returns false if max
	bool decreaseSwingThreshold(); // returns false if min
	bool isSwinging();
  void swingTrace();

	//bool isThrusting(); // one day...
	//bool isSpinning(); // eventually...
	
	private:
	float getX();
    float getY();
    float getZ();
	float getGravity(uint16_t reading);
	uint8_t getSquared();
  float getDeltaSum();

	uint8_t _swingThreshold = 16; //empirical testing
	uint8_t _pin_x;
    uint8_t _pin_y;
    uint8_t _pin_z;
	float _aref;
	float _bias;
	float _xg;
    float _yg;
    float _zg;
    float _mvG;
    float _rad2deg;
 uint32_t _swing_start_time;
 float _minX;
 float _maxX;
 float _minY;
 float _maxY;
 float _minZ;
 float _maxZ;
 float _deltaX;
 float _deltaY;
 float _deltaZ;
	
};

#endif


