#include "ColorGem.h"
ColorGem::ColorGem(uint8_t analog_pin, uint8_t digital_pin){
  _analog_pin = analog_pin;
	_digital_pin = digital_pin;
	_red = 0;
	_green = 0;
	_blue = 0;
  pinMode(_digital_pin, OUTPUT);
  digitalWrite(_digital_pin, HIGH);
}
void ColorGem::readGem(){
	uint16_t value = getValue();

  
//10kohm + 560ohmin parallell to ground
/*
ljusgrön 922 926 929
orage 912 917 921
gul 899 905 909
lila 886 892 896
vit 868 875 879
rosa 823 832 838
ljusblå 763 773 781
mörkblå 690 701 712
grön 599 612 625
röd 378 388 406

925
915
902.5
887.5
858.5
809.5
746.5
668.5
515.5
*/
	
	if (value<=515)
	{
  // red
		_red = 255;
		_green = 0;
		_blue = 0;
	}
	else if (value>515&&value<=668)
	{
  // green
		_red = 128;
		_green = 255;
		_blue = 0;
	}
	else if (value>668&&value<=746)
	{
    // dark blue
		_red = 0;
		_green = 0;
		_blue = 255;
	}
	else if (value>746&&value<=809)
	{
  //light blue
		_red = 50;
		_green = 50;
		_blue = 255;
	}
	else if (value>809&&value<=858)
	{
  // pink
		_red = 255;
		_green = 100;
		_blue = 80;
	}
	else if (value>858&&value<=887)
	{
  // white
		_red = 150;
		_green =150;
		_blue = 50;
	}
	else if (value>887&&value<=902)
	{
  // purple
		_red = 150;
		_green = 0;
		_blue = 130;
	}
	else if (value>902&&value<=915)
	{
  // gul
		_red = 255;
		_green = 160;
		_blue = 0;
	}
	else if (value>915&&value<=925)
	{
  //orange
		_red = 240;
		_green = 20;
		_blue = 0;
	}
 else if (value>925)
 {
  // light green
    _red = 50;
    _green = 255;
    _blue = 10;
  }
  digitalWrite(_digital_pin, LOW);
}
uint16_t ColorGem::getValue(){
    return analogRead(_analog_pin);
}



