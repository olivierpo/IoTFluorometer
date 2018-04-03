
#include "FluoKeypad.h"

void FluoKeypad::init() {
  my_keypad = new Keypad(makeKeymap(keymap), row_pins, col_pins, ROWS, COLS);
}

char FluoKeypad::getKeyPressed() {
  return my_keypad->getKey();
}

void FluoKeypad::reconfigurePins() {
  byte i;

  // go back to all pins as per the keypad library

  for (i = 0; i < COLS; i++){
    pinMode (col_pins [i], OUTPUT);
    digitalWrite (col_pins [i], HIGH);
  }  // end of for each column

  for (i = 0; i < ROWS; i++){
    pinMode (row_pins [i], INPUT_PULLUP);
  }   // end of for each row
}

void FluoKeypad::prepareSleep() {
  byte i;

  // set up to detect a keypress
  for (i = 0; i < COLS; i++){
    pinMode (col_pins [i], OUTPUT);
    digitalWrite (col_pins [i], LOW);   // columns low
  }  // end of for each column

  for (i = 0; i < ROWS; i++){
    pinMode (row_pins [i], INPUT_PULLUP);
  }  // end of for each row

  // now check no pins pressed (otherwise we wake on a key release)
  for (i = 0; i < ROWS; i++){
    if (digitalRead (row_pins [i]) == LOW){
      reconfigurePins ();
      return;
    } // end of a pin pressed
  }  // end of for each row

  // overcome any debounce delays built into the keypad library
  delay (50);
}

