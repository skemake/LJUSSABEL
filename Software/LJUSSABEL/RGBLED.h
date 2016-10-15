#ifndef RGBLED_h
#define RGBLED_h

// assuming 8 bit PWM (I'm using uint8_t for the colour values).

#include <Arduino.h>
enum RGBLEDState {
  off,
  on,
  fadeingIn,
  fadeingOut,
  cycling
};

class RGBLED
{
  public:
  RGBLED(uint8_t redPin, uint8_t greenPin, uint8_t bluePin);
  RGBLEDState getState();
  void poke();
  bool turnOn();
  bool turnOff();
  bool fadeIn(uint16_t milliseconds);
  bool fadeOut(uint16_t milliseconds);
  bool cycle(uint16_t milliseconds); // TODO
  void blink(uint16_t milliseconds, uint8_t times=1); // TODO
  void setFlicker(uint8_t amount); // TODO
  bool flash(uint8_t red, uint8_t green, uint8_t blue);
  void setValue(uint8_t red, uint8_t green, uint8_t blue);
  
  private:
  const uint8_t _lookup[33] = {0,1,2,3,4,5,7,9,11,14,17,21,25,30,35,41,47,54,62,70,79,89,99,111,123,136,150,165,181,198,216,235,255};
  
  void setRGBColour(uint8_t red, uint8_t green, uint8_t blue);
  RGBLEDState _state;
  uint32_t _previousMillis = 0;
  uint32_t _interval = 1;
  uint8_t _flicker = 0;
  uint8_t _redpin;
  uint8_t _greenpin;
  uint8_t _bluepin;
  uint8_t _rgbValue[3]; // holds target color values
  int16_t _rgbColour[3]; // holds current led values //cant be unsigned, flash will not balance itself then.
  uint16_t _fadeDuration = 2000;
  uint16_t _fadeIter = 0;
  
  uint8_t _decColour = 0;
  uint8_t _incColour = 1;
};
#endif


