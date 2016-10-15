#ifndef SOUNDFONT_h
#define SOUNDFONT_h
#include <Arduino.h>

// first value in the array is the # of soundclips
// second value in the array is the last played sound (we dont like repeating the same sound if we have a choice)
// third value and forward is the address of the playlist in the WT588D
/*uint8_t fx_hum[] = {1,0,0};
uint8_t fx_poweron[] = {1, 0, 1,};
uint8_t fx_poweroff[] = {1, 0, 2};
uint8_t fx_swing[] = {1, 0, 3};
uint8_t fx_clash[] = {1, 0, 4};
uint8_t fx_boot[] = {1, 0, 5};
uint8_t fx_blaster[] = {1, 0, 6};
uint8_t fx_lockup[] = {1, 0, 7};
*/

uint8_t fx_hum[] = {1,0,0x00};
uint8_t fx_poweron[] = {4, 0, 0x01,0x02,0x03,0x04};
uint8_t fx_poweroff[] = {2, 0, 0x05,0x06};
//uint8_t fx_swing[] = {16, 0, 0x07,0x08,0x9,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10,0x11,0x12,0x13,0x14,0x15,0x16};
uint8_t fx_swing[] = {21, 0, 0x07,0x08,0x0b,0x0c,0x0d,0x0e,0x0f,0x10,0x11,0x12,0x13,0x14,0x16,0x0d,0x0d,0x0d,0x0d,0x11,0x11,0x11,0x11};
uint8_t fx_clash[] = {16, 0, 0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,0x20,0x21,0x22,0x23,0x24,0x25,0x26};
uint8_t fx_boot[] = {1, 0, 0x27};
uint8_t fx_blaster[] = {4, 0, 0x28,0x29,0x2a,0x2b};
uint8_t fx_combo[] = {3, 0, 0x2d,0x2e,0x2f};
uint8_t fx_stab[] = {4,0, 0x30,0x31,0x32,0x33};
uint8_t fx_lockup[] = {1, 0, 0x2c};

uint8_t getRandom(uint8_t* soundfont) {
	if(soundfont[0]==1)	{ // If we only got one sound in this font, just return it
		return soundfont[2];
	} 
	uint8_t rndindex = random(2, 2 + soundfont[0]);
	uint8_t lastSound = soundfont[1];

  // Re-randomize as long as we get same random number as last time
  while (rndindex == lastSound)
  {
    rndindex = random(2, 2 + soundfont[0]);
  }

  // Save sound so we can avoid it next time
  soundfont[1]=rndindex;
	
  return soundfont[rndindex];
}
#endif


