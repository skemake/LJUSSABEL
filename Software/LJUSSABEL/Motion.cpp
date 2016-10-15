#include "Motion.h"

//#define DEBUG
#include "Debug.h"


Motion::Motion(uint8_t pin_x, uint8_t pin_y, uint8_t pin_z, float aref, uint8_t pin_vcc, uint8_t pin_gnd){
	pinMode(pin_gnd, OUTPUT);
	digitalWrite(pin_gnd, LOW);
	pinMode(pin_vcc, OUTPUT);
	digitalWrite(pin_vcc, HIGH);
	
	_aref = aref;
	_pin_x = pin_x;
	_pin_y = pin_y;
	_pin_z = pin_z;
	_mvG = aref / 10.0;
	_bias = aref / 2.0;
	_rad2deg = 180.0 / M_PI;
  _swing_start_time = millis();
  _deltaX=0;
  _deltaY=0;
  _deltaZ=0;
}
void Motion::update(){
	_xg = getGravity(analogRead(_pin_x));
	_yg = getGravity(analogRead(_pin_y));
	_zg = getGravity(analogRead(_pin_z));

  if (millis() - _swing_start_time > SWING_SAMPLE_TIME)
  {
    _deltaX = _maxX - _minX;
    _deltaY = _maxY - _minY;
    _deltaZ = _maxZ - _minZ;
    
    _minX=_xg;
    _maxX=_xg;
    _minY=_yg;
    _maxY=_yg;
    _minZ=_zg;
    _maxZ=_zg;
    
    _swing_start_time = millis();
  }
  if(_xg < _minX) _minX = _xg;
  if(_xg > _maxX) _maxX = _xg;
  if(_yg < _minY) _minY = _yg;
  if(_yg > _maxY) _maxY = _yg;
  if(_zg < _minZ) _minZ = _zg;
  if(_zg > _maxZ) _maxZ = _zg;

}

float Motion::getDeltaSum()
{
  return (_deltaX + _deltaY + _deltaZ);
}

float Motion::getGravity(uint16_t reading)
{
  float voltage = reading * _aref;
  voltage /= 1024.0;

  //minus the zero g bias
  //then divide by mv/g
  //which when Vs = 3.3V, V/g = 0.330
  float gv = (voltage - _bias) / _mvG;

  return gv;
}

void Motion::swingTrace()
{
  trace(_deltaX);
  trace(_deltaY);
  trace(_deltaZ);
}

bool Motion::isSwinging()
{
  return ((getDeltaSum() >= 1) && (getDeltaSum() < 3));
		//return (getSquared() >= _swingThreshold);
}

bool Motion::increaseSwingThreshold(){
	if(_swingThreshold==SWING_THRESHOLD_MAX){
		return false;
	}else{
		_swingThreshold++;
		return true;
	}
}
bool Motion::decreaseSwingThreshold(){
	if(_swingThreshold==SWING_THRESHOLD_MIN){
		return false;
	}else{
		_swingThreshold--;
		return true;
	}
}
uint8_t Motion::getSquared(){
	return ((_xg*_xg)+(_yg*_yg)+(_zg*_zg));
}

float Motion::getX()
{
  return _xg;
}

float Motion::getY()
{
  return _yg;
}

float Motion::getZ()
{
  return _zg;
}


