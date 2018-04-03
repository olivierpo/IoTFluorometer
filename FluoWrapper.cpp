#include <FluoLocalStore.h>
#include <FluoLowPower.h>
#include <FileIO.h>
#include <Bridge.h>
#include "FluoWrapper.h"

void FluoWrapper::init(uint16_t sleep_t, bool can_sl, uint16_t draw_int, uint16_t send_int,
                       uint8_t time_b_s, bool can_se, bool can_st, bool size_c_e, unsigned long size_c, bool send_int_e, String path_to_f) {
  last_action_time = 0;
  time_pro_sleep = 0;
  wokeup = true;
  prev_multiple;
  slept_time_millis = 0;
  lost_time = 0;
  has_init = false;
  input_mode = false;
  prev_print_time = 1000;

  sleep_time = sleep_t;
  can_sleep = can_sl;
  draw_interval = draw_int;
  send_interval = send_int;
  time_before_sleep = time_b_s;
  can_store = can_st;
  can_send = can_se;
  size_cap_enabled = size_c_e;
  size_cap = size_c;
  send_interval_enabled = send_int_e;
  path_to_file = path_to_f;
}

void FluoWrapper::initP2() {
  pad = new FluoKeypad(13, 12, 11, 10, 9, 8, 7, 6);
  oled = new FluoOLED(4);
  oled->begin();
  oled->print("Init bridge... ", sizeof("Init bridge... "));
  Serial.begin(9600);
  while (!Serial);
  Serial.print(F("Initializing the bridge... ")); //Serial print for debug purposes
  Bridge.begin();
  Serial.println(F("Done!\n"));
  Serial.println(path_to_file);
  pad->init();
  FileSystem.begin();
  FileSystem.remove(path_to_file.c_str());
  oled->clear();
  calibrateTime(millis());
  has_init = true;
}

uint16_t FluoWrapper::calibrateTime(double missed_time) {
  lost_time += missed_time;
  return (millis() - lost_time) / 1000;
}

void FluoWrapper::markAction() {
  if (!wokeup) {
    calibrateTime(millis() - time_pro_sleep);
  }
  wokeup = true;
  oled->exitSleepMode();
  last_action_time = calibrateTime(0);
}

void FluoWrapper::managePowerSave() {
  if (!has_init) {
    initP2();
  }
  if (can_sleep) {
    if (calibrateTime(0) >= last_action_time + time_before_sleep) {
      if (wokeup) {
        time_pro_sleep = millis();
      }
      wokeup = false;
      FluoLowPower power;
      Serial.println(F("Using sleep object..."));
      Serial.flush();
      //oled->enterSleepMode();
      pad->prepareSleep();
      power.powerSaveModeOn(sleep_time);
      pad->reconfigurePins();
      can_sleep = false; 
      Serial.begin(9600);
      while(!Serial);
    }
  }
}

void FluoWrapper::collectAndStoreData() {
  if (!has_init) {
    initP2();
  }
  FluoLocalStore store;
  int curr_multiple = (calibrateTime(0));
  double time_before_send;
  if (can_send && can_store) {
    if ((curr_multiple % send_interval == 0 && prev_multiple != curr_multiple && send_interval_enabled)
        || (size_cap_enabled && store.getFileSize(path_to_file) >= size_cap)) {
      time_before_send = millis();
      oled->clear();
      oled->setCursor(0, 0);
      oled->print("Sending to Sheets...", sizeof("Sending to Sheets..."));
      cloud_client.sendToCloudFromStorage(path_to_file);
      calibrateTime(millis() - time_before_send);
      store.deleteFile(path_to_file);
      oled->clear();
    }
  }
  if ( curr_multiple % draw_interval == 0 && prev_multiple != curr_multiple && can_store) {
    time_before_send = millis();
    //Serial.println(F("usb1: " + String(calibrateTime(0)));
    store.saveToUSB(String(millis()), path_to_file);
    calibrateTime(millis() - time_before_send);
    //Serial.println(F("usb2: " + String(calibrateTime(0)));
    prev_multiple = curr_multiple;
  }
  if (!can_store && can_send) {
    cloud_client.sendToCloudDirect(String(calibrateTime(0)));
  }
}

void FluoWrapper::displayTime() {
  if (!has_init) {
    initP2();
  }
  uint16_t curr_time_sec = calibrateTime(0);
  if (curr_time_sec != prev_print_time) {
    oled->setCursor(1, 8);           // move cursor to second line "1" and 9 spaces over
    String curr_time_str = String(curr_time_sec);
    byte char_array[curr_time_str.length() + 1];
    curr_time_str.toCharArray(char_array, curr_time_str.length() + 1);
    oled->print(char_array, sizeof(char_array));  // display seconds elapsed since power-up (minus time spent asleep and other wasted time)
    prev_print_time = curr_time_sec;
    Serial.println(curr_time_sec);
    freeRAM();
  }
}

void FluoWrapper::manageButtons() {
  if (!has_init) {
    initP2();
  }
  //FluoKeypad pad = FluoKeypad(pad_pins[0], pad_pins[1], pad_pins[2], pad_pins[3], pad_pins[4], pad_pins[5], pad_pins[6]);
  char pad_key = pad->getKeyPressed();  // read the buttons
  //markAction();
  if (pad_key == '*') {
    markAction();
    oled->setCursor(0, 0);
    if (!input_mode) {
      enterInputMode();
    }
    else {
      oled->print("*", sizeof("*"));
    }
  }
  else if (pad_key == '#') {
    markAction();
    if (input_mode) {
      exitInputMode();
    }
    else {
      oled->clear();
    }
  }
  else if (input_mode) {
    if (pad_key != NO_KEY) {
      oled->setCursor(0, 0);
      markAction();
    }
    if (pad_key == '1') {
      oled->print("1", sizeof("*"));
    }
    else if (pad_key == '2') {
      oled->print("2", sizeof("*"));
    }
    else if (pad_key == '3') {
      oled->print("3", sizeof("*"));
    }
    else if (pad_key == '4') {
      oled->print("4", sizeof("*"));
    }
    else if (pad_key == '5') {
      oled->print("5", sizeof("*"));
    }
    else if (pad_key == '6') {
      oled->print("6", sizeof("*"));
    }
    else if (pad_key == '7') {
      oled->print("7", sizeof("*"));
    }
    else if (pad_key == '8') {
      oled->print("8", sizeof("*"));
    }
    else if (pad_key == '9') {
      oled->print("9", sizeof("*"));
    }
    else if (pad_key == '0') {
      oled->print("0", sizeof("*"));
    }
    else if (pad_key == 'A') {
      oled->print("A", sizeof("*"));
    }
    else if (pad_key == 'B') {
      oled->print("B", sizeof("*"));
    }
    else if (pad_key == 'C') {
      oled->print("C", sizeof("*"));
    }
    else if (pad_key == 'D') {
      oled->print("D", sizeof("*"));
    }
  }
}

// Returns the number of bytes currently free in RAM
static void FluoWrapper::freeRAM(void)
{
    extern int __heap_start, *__brkval; 
  int v; 
  Serial.println(String( (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval))); 
  
}
