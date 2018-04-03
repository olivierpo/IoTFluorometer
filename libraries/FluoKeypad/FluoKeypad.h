/**
  Library for using the Membrane 3x4 Matrix Keypad.
**/

#ifndef IOTFLUOROMETER_FLUOKEYPAD_H_
#define IOTFLUOROMETER_FLUOKEYPAD_H_

#include <Arduino.h>
#include <Keypad.h>

#define ROWS 4
#define COLS 4

class FluoKeypad {
  private:
    Keypad *my_keypad;
    byte row_pins[ROWS];
    byte col_pins[COLS];
    const PROGMEM char keymap[ROWS][COLS] =
    {
      {'1', '2', '3', 'A'},
      {'4', '5', '6', 'B'},
      {'7', '8', '9', 'C'},
      {'*', '0', '#', 'D'}
    };
  public:
    FluoKeypad(byte pin7, byte pin6, byte pin5, byte pin4, byte pin3, byte pin2, byte pin1, byte pin0)
      : row_pins{pin7, pin6, pin5, pin4}, col_pins{pin3, pin2, pin1, pin0} {
      PCMSK0 |= bit (PCINT2);   // pin 6
      PCMSK0 |= bit (PCINT3);   // pin 7
      PCMSK0 |= bit (PCINT4);    // pin 8
      PCMSK0 |= bit (PCINT5);    // pin 9
    }
    ~FluoKeypad() {
      delete my_keypad;
    }
    void reconfigurePins();
    void prepareSleep();
    void init();
    char getKeyPressed();
};

#endif // IOTFLUOROMETER_FLUOKEYPAD_H_

