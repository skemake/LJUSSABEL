#include "RGBLED.h"

RGBLED::RGBLED(uint8_t redPin, uint8_t greenPin, uint8_t bluePin){
	_redpin = redPin;
	_greenpin = greenPin;
	_bluepin = bluePin;
	_state =off;
	
}
void RGBLED::setValue(uint8_t red, uint8_t green, uint8_t blue){
	_rgbValue[0] = red;
	_rgbValue[1] = green;
	_rgbValue[2] = blue;
    
}
 RGBLEDState RGBLED::getState(){
  return _state;
 }
void RGBLED::poke(){
	unsigned long currentMillis = millis();
	if(currentMillis - _previousMillis >= _interval) {
		_previousMillis = currentMillis;
		if (_state == on){
            // restore to color after flash
            for(int i=0;i<3;i++){
               //if ((_rgbColour[i]-_rgbValue[i])<10||(_rgbColour[i]-_rgbValue[i])>10){
               // _rgbColour[i]=_rgbValue[i];
              // }
               
                if(_rgbColour[i]>_rgbValue[i]){
                    _rgbColour[i]=_rgbColour[i] - 6; // if the flash is too slow fadeing down, increase this value
                } else if (_rgbColour[i]<_rgbValue[i]) {
                    _rgbColour[i]=_rgbColour[i] + 5; // not the same as the reducing part above, the idea is that it will stabilize itself.
                } else {
                   _rgbColour[i] = _rgbColour[i] - random(0, _flicker);
                }
            }
        }else if (_state == fadeingIn){
			if (_fadeIter<=32){
				for(int i=0;i<3;i++){
                    _rgbColour[i] = (uint8_t)(_rgbValue[i]/(float)255.0*_lookup[_fadeIter]);
				}
                _fadeIter++;
			}else{
				_state = on;
			}
		} else if (_state == fadeingOut){
			if (_fadeIter>0){
				for(int i=0;i<3;i++){
                    _rgbColour[i] = (uint8_t)(_rgbValue[i]/(float)255.0*_lookup[_fadeIter]);
				}
       _fadeIter--;
			}else{
        _rgbColour[0] = 0;
        _rgbColour[1] = 0;
        _rgbColour[2] = 0;
				_state = off;
			}
		} else if (_state == cycling){
			// Hue cycle
			if (_fadeIter<=32){
				_rgbColour[0] = 255;
				_rgbColour[1] = _lookup[_fadeIter];
				_rgbColour[2] = 0;
			}else if (_fadeIter>32 && _fadeIter<=65){
				_rgbColour[0] = _lookup[32-(_fadeIter-33)];
				_rgbColour[1] = 255;
				_rgbColour[2] = 0;
			}else if (_fadeIter>65 && _fadeIter<=98){
				_rgbColour[0] = 0;
				_rgbColour[1] = 255;
				_rgbColour[2] = (_lookup[_fadeIter-66]);
			}else if (_fadeIter>98 && _fadeIter<=131){
				_rgbColour[0] = 0;
				_rgbColour[1] = (_lookup[32-(_fadeIter-99)]);
				_rgbColour[2] = 255;	
			}else if (_fadeIter>131 && _fadeIter<=164){
				_rgbColour[0] = (_lookup[_fadeIter-132]);
				_rgbColour[1] = 0;
				_rgbColour[2] =  255;
			}else if (_fadeIter>164 && _fadeIter<=197){
				_rgbColour[0] = 255;
				_rgbColour[1] =  0;
				_rgbColour[2] = (_lookup[32-(_fadeIter-165)]);	
			}
			 _fadeIter++;
			if (_fadeIter>197){
				_fadeIter=0;
			}
		}
	}    
    setRGBColour(_rgbColour[0], _rgbColour[1], _rgbColour[2]);
}
void RGBLED::setRGBColour(uint8_t red, uint8_t green, uint8_t blue){
	analogWrite(_redpin, red);
	analogWrite(_greenpin, green);
	analogWrite(_bluepin, blue);
}
bool RGBLED::turnOn(){
	if(_state == off){
		_rgbColour[0] = _rgbValue[0];
		_rgbColour[1] = _rgbValue[1];
		_rgbColour[2] = _rgbValue[2];
		_state = on;
		return true;
	}else{
		return false; // Can't turn on, busy doing something else.
	}
}
bool RGBLED::turnOff(){
  _rgbColour[0] = 0;
	_rgbColour[1] = 0;
  _rgbColour[2] = 0;
  setRGBColour(_rgbColour[0], _rgbColour[1], _rgbColour[2]);
  _state = off;
	return true; // If already off, return true anyway.
}
bool RGBLED::fadeIn(uint16_t milliseconds){
	if(_state == off){
		_fadeDuration = milliseconds;
        _interval = milliseconds/33;
        _fadeIter = 0;
		_state = fadeingIn;
		return true;
	}else{
		return false; // Can't start cycle now, busy doing something else.
	}
}
bool RGBLED::cycle(uint16_t milliseconds){
	if(_state == off || _state == on){
		_fadeDuration = milliseconds;
        _interval = milliseconds/198;
        _fadeIter = 0;
		_state = cycling;
		return true;
	}else{
		return false; // Can't turn on, busy doing something else.
	}
}
bool RGBLED::fadeOut(uint16_t milliseconds){
	if(_state == on){
		_fadeDuration = milliseconds;
        _interval = milliseconds/33;
        _fadeIter = 32;
		_state = fadeingOut;
		return true;
	}else{
		return false; // Can't turn on, busy doing something else.
	}
}
void RGBLED::setFlicker(uint8_t amount){
  _flicker = amount;
}

bool RGBLED::flash(uint8_t red, uint8_t green, uint8_t blue){
    if(_state == on){
		_rgbColour[0] = red;
		_rgbColour[1] = green;
		_rgbColour[2] = blue;
		_interval = 1;
		return true;
	} else {
		return false;
	}
}



