
#include "OneLineWT588D.h"

// Sound chip
#define WT588D_DATA_PIN 4
#define WT588D_BUSY_PIN 7
#define WT588D_RESET_PIN 11

OneLineWT588D Soundchip(WT588D_DATA_PIN,WT588D_BUSY_PIN,WT588D_RESET_PIN);

bool readyOk = false;

void setup()
{
  Soundchip.warmUp();
}

void checkReadyOk()
{
  if(!readyOk)
  {
    delay(500);
    Soundchip.play(0x2c); //"lockup" sound
    delay(500);
    while(Soundchip.isBusy() == true)
    {
      delay(10);
    }
    readyOk = true;
  }
}
void loop()
{
  checkReadyOk();
  for(int i = 0;i<=51;i++)
  {
    Soundchip.play(i);
    delay(1000);
  }
}

