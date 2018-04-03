/**
	Low Power library for Arduino Yun.
**/

#ifndef IOTFLUOROMETER_FLUOLOWPOWER_H_
#define IOTFLUOROMETER_FLUOLOWPOWER_H_

#define DISABLE_TIMER1 0
#define DISABLE_TIMER2 0
#define DISABLE_SERIAL 0
#define DISABLE_ADC 0
#define DISABLE_WIRE 0
#define DISABLE_SPI 0

#include <Arduino.h>

class FluoLowPower {
  public:
    void powerSaveModeOn(uint8_t);
    void powerSaveModeOff();
  private:
    const uint8_t SleepMode = SLEEP_MODE_PWR_DOWN;
    void sleepNow(uint8_t);
    void sleepNowInt();
};

#endif // IOTFLUOROMETER_FLUOLOWPOWER_H_

