#ifndef ONELINEWT588D_h
#define ONELINEWT588D_h

#include <Arduino.h>

class OneLineWT588D
{
  public:
    OneLineWT588D(uint8_t dataPin, uint8_t busyPin, uint8_t resetPin);
    void warmUp();
    bool turnItUp();
    bool turnItDown();
    void play(uint8_t playlist);
    void stop();
    void repeat();
    bool isBusy();

  private:
    uint8_t _volume = 0xE7; // start volume
    uint8_t _dataPin, _busyPin, _resetPin;
    void sendOneLineCommand(uint8_t addr);
};

#endif


