#include "OneLineWT588D.h"

#define WT588D_MINVOLUME 0xE0
#define WT588D_MAXVOLUME 0xE7
#define WT588D_STARTREPEAT 0xF2
#define WT588D_STOP 0xFE

OneLineWT588D::OneLineWT588D(uint8_t dataPin, uint8_t busyPin,uint8_t resetPin)
{
  _dataPin = dataPin;
  _busyPin = busyPin;
  _resetPin = resetPin;
}

void OneLineWT588D::warmUp()
{
  pinMode(_dataPin,OUTPUT);
  pinMode(_busyPin,INPUT);
  pinMode(_resetPin,OUTPUT);
  digitalWrite(_resetPin, LOW);
  delay(100);  // at least 5 according to spec
  digitalWrite(_resetPin, HIGH);
  delay(20); // at least 17 according to spec

  // set volume to default (minimum in this case)
  sendOneLineCommand(_volume);
}

bool OneLineWT588D::turnItUp()
{
  if(_volume < WT588D_MAXVOLUME)
  {
    _volume++;
  }
  else
  {
    return false;
  }
  sendOneLineCommand(_volume);
  return true;
}

bool OneLineWT588D::turnItDown()
{
  if(_volume > WT588D_MINVOLUME)
  {
    _volume--;
  }
  else
  {
    return false;
  }
  sendOneLineCommand(_volume);
  return true;
}

void OneLineWT588D::play(uint8_t playlist)
{
  sendOneLineCommand(playlist);
}

void OneLineWT588D::stop()
{
  sendOneLineCommand(WT588D_STOP);
}

void OneLineWT588D::repeat()
{
  sendOneLineCommand(WT588D_STARTREPEAT);
}

bool OneLineWT588D::isBusy()
{
  return digitalRead(_busyPin)==HIGH;
}

void OneLineWT588D::sendOneLineCommand(uint8_t addr)
{
  digitalWrite(_dataPin, LOW);
  delay(5);

  for(int i = 0; i < 8; i++)
  {
    digitalWrite(_dataPin, HIGH);
    if(bitRead(addr, i))
    {
      delayMicroseconds(400);
      digitalWrite(_dataPin, LOW);
      delayMicroseconds(200);
    }
    else
    {
      delayMicroseconds(200);
      digitalWrite(_dataPin, LOW);
      delayMicroseconds(400);
    }
  }

  digitalWrite(_dataPin, HIGH);
  delay(2);
}
