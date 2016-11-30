#include "OneButton.h"
#include "RGBLED.h"
#include "OneLineWT588D.h"
#include "Motion.h"
#include "SoundFont.h"
#include "SM.h"
#include "ColorGem.h"

// Uncomment next line to enable debug traces
//#define DEBUG
#include "Debug.h"

// Button
#define SW_ACT_PIN 8

// LED PWM pins
#define LED_RED_PIN 9
#define LED_GREEN_PIN 6
#define LED_BLUE_PIN 5

// Unused PWM pins for extra LED
//#define LED_ACCENT_RED_PIN 10
//#define LED_ACCENT_GREEN_PIN 3
//#define LED_ACCENT_BLUE_PIN 12

// Sound chip
#define WT588D_DATA_PIN 4
#define WT588D_BUSY_PIN 7
#define WT588D_RESET_PIN 11

// Accelerometer
#define GY_61_GND_PIN 13
#define GY_61_VCC_PIN A4
#define GY_61_X_PIN A2
#define GY_61_Y_PIN A1
#define GY_61_Z_PIN A0

// Clash switch
#define CLASH_PIN 2

// Crystal pins (voltage divider circuit)
#define COLOR_GEM_ANALOG_PIN A3
#define COLOR_GEM_DIGITAL_PIN 12

// Used to get random seed for sound selection
#define RANDOMSEED_PIN A5

// Global for clash sensor debounce
uint32_t clash_interrupt_time;
// Clash interrupt flag
bool clashInterruptTriggered=false;
// Do not trigger swings directly after clash
#define CLASH_SWING_OVERRIDE_DURATION 1000

// Global for hum restart timing
unsigned long latestActivityMillis=0L;
unsigned long millisNow=0L;
#define HUM_RESTART_INTERVAL 2000 

// Minimum swing sound time
#define SWING_SOUND_DURATION 1000
uint32_t swing_interrupt_time;

// Init
OneButton ButtonAct(SW_ACT_PIN, true);
RGBLED Led(LED_RED_PIN,LED_GREEN_PIN,LED_BLUE_PIN);
OneLineWT588D Soundchip(WT588D_DATA_PIN,WT588D_BUSY_PIN,WT588D_RESET_PIN);
Motion Accelerometer(GY_61_X_PIN, GY_61_Y_PIN, GY_61_Z_PIN, 3.3, GY_61_VCC_PIN, GY_61_GND_PIN);
ColorGem BladeGem(COLOR_GEM_ANALOG_PIN, COLOR_GEM_DIGITAL_PIN);

//Set state machine to initial state.
State waitStateRunOnce(); // preallocate
State waitState(); // preallocate
SM Machine(waitStateRunOnce, waitState); // create state machine
void enterWaitState(){ Machine.Set(waitStateRunOnce, waitState); }
void enterLitState(){ Machine.Set(litStateRunOnce, litState); }
void enterLockupState(){ Machine.Set(lockupStateRunOnce, lockupState); }

void setup()
{
  trace_begin(115200);
  trace("Blade initializing...");

  Soundchip.warmUp();

  ButtonAct.setClickTicks(150); //600 default (debounce 60)
  ButtonAct.setPressTicks(500); // 1000 default

  // clash sensor
  digitalWrite (CLASH_PIN, HIGH);
  attachInterrupt(digitalPinToInterrupt(CLASH_PIN), clashInterrupt, FALLING);
  clash_interrupt_time = millis();

  swing_interrupt_time = millis();

  delay(200);

  // Init blade color
  trace(BladeGem.getValue());
  BladeGem.readGem();
  Led.setValue(BladeGem.getRed(),BladeGem.getGreen(),BladeGem.getBlue());

  //set seed for the random() in SoundFont.h
  randomSeed(analogRead(RANDOMSEED_PIN));
  enterWaitState();
}

void loop()
{
  // Update everything
  Accelerometer.update();
  ButtonAct.tick();
  Led.poke();
  EXEC(Machine);
  millisNow=millis();
}

State waitStateRunOnce()
{
  trace("Enter wait state.");
  // set button function pointers
  ButtonAct.attachClick(enterLitState);

  // disable other functions
  ButtonAct.attachDoubleClick(buttonDisabled);
  ButtonAct.attachLongPressStart(buttonDisabled);
  ButtonAct.attachDuringLongPress(buttonDisabled);
  ButtonAct.attachLongPressStop(buttonDisabled);

  // led off
  if(Led.fadeOut(500)) // returns true if led was on
  {
    Soundchip.play(getRandom(fx_poweroff));
  }
  else
  {
    Soundchip.play(getRandom(fx_boot));
    Led.turnOff();
  }
}

State waitState()
{
}

State litStateRunOnce()
{
  trace("Enter lit state.");

  // set button function pointers
  ButtonAct.attachClick(enterWaitState); // turn led off
  ButtonAct.attachLongPressStart(enterLockupState);

  // disable all else
  ButtonAct.attachDoubleClick(buttonDisabled);
  ButtonAct.attachDuringLongPress(buttonDisabled);
  ButtonAct.attachLongPressStop(buttonDisabled);

  // Clear any clash interrupts that happened in lockup state
  clashInterruptTriggered = false;

  Led.setFlicker(0);

  if(Led.fadeIn(500)) // returns true if led was off
  {
    Soundchip.play(getRandom(fx_poweron));
    latestActivityMillis=millisNow;
  }
  else
  { // led is on, so we should be humming already (e.g. returning from lockup)
    Soundchip.play(getRandom(fx_hum));
    Soundchip.repeat();
  }
}

State litState()
{
  if(Accelerometer.isSwinging())
  {
    swing();
    latestActivityMillis=millisNow;
  }

  if(clashInterruptTriggered)
  {
    clash();
    latestActivityMillis=millisNow;
  }

  // Restart humming if we have silence and there has been some time since last activity
  if (((unsigned long)(millisNow - latestActivityMillis) >= HUM_RESTART_INTERVAL) &&(Soundchip.isBusy() == false))
  {
    latestActivityMillis=millisNow;
    trace("Restart humming");
    Soundchip.play(getRandom(fx_hum));
    Soundchip.repeat();
  }
}

State lockupStateRunOnce()
{
  trace("Enter lockup state.");

  // set button function pointers
  ButtonAct.attachClick(enterWaitState); // turn led off
  ButtonAct.attachLongPressStop(enterLitState);

  // disable all else
  ButtonAct.attachDoubleClick(buttonDisabled);
  ButtonAct.attachLongPressStart(buttonDisabled);
  ButtonAct.attachDuringLongPress(buttonDisabled);

  Soundchip.play(getRandom(fx_lockup));
  Soundchip.repeat();
  latestActivityMillis=millisNow;
}

State lockupState()
{
  Led.setFlicker(5);
}

/*
void deflect(){
  Led.flash(255,255,255);
  Soundchip.play(getRandom(fx_blaster));
}*/

void swing()
{
  if ((millis() - clash_interrupt_time) < CLASH_SWING_OVERRIDE_DURATION)
  {
    // Do not allow swings directly after clash, since the blade bounce oftern trigger flase swings
    trace("swing clash override");
  }
  else
  {
    if (millis() - swing_interrupt_time > SWING_SOUND_DURATION)
    {
      trace("Swing.");
      Accelerometer.swingTrace();
      Soundchip.play(getRandom(fx_swing));
      swing_interrupt_time = millis();
    }
  }
}

void clash()
{
  trace("Clash.");
  Led.flash(255,255,255);
  Soundchip.play(getRandom(fx_clash));
  clashInterruptTriggered = false;
}

void clashInterrupt()
{
  if(Led.getState()==on)
  {
    if (millis() - clash_interrupt_time > 200)
    {
      clashInterruptTriggered = true;
      trace("interrupt");
      clash_interrupt_time = millis();
    }
  }
}

void buttonDisabled()
{
  // do nothing
}
