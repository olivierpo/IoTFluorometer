#include <LowPower.h>
//#include <Narcoleptic.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include "FluoLowPower.h"

ISR (PCINT0_vect)
  {
  PCICR = 0;  // cancel pin change interrupts
  } // end of ISR (PCINT0_vect)

ISR (PCINT1_vect)
  {
  PCICR = 0;  // cancel pin change interrupts
  } // end of ISR (PCINT1_vect)

ISR (PCINT2_vect)
  {
  PCICR = 0;  // cancel pin change interrupts
  } // end of ISR (PCINT2_vect)
  
void FluoLowPower::sleepNow(uint8_t sleep_t) {
#ifdef BODSE
  // Turn off BOD in sleep (Arduino mega 2560 BOD triggers a RESET on activation)
  MCUCR |= _BV(BODSE);
  MCUCR |= _BV(BODS);
#endif

  MCUSR = 0; //clear the MCU status register
#ifdef WDTCSR
  WDTCSR &= ~_BV(WDE);
  WDTCSR = _BV(WDIF) | _BV(WDIE) | _BV(WDCE);
#else
  WDTCR &= ~_BV(WDE);
  WDTCR = _BV(WDIF) | _BV(WDIE) | _BV(WDCE);
#endif
  uint8_t sleep_time;
  switch (sleep_t) {
    case 15:
      {
        sleep_time = WDTO_15MS;
      }
    case 30:
      {
        sleep_time = WDTO_30MS;
      }
    case 60:
      {
        sleep_time = WDTO_60MS;
      }
    case 120:
      {
        sleep_time = WDTO_120MS;
      }
    case 250:
      {
        sleep_time = WDTO_250MS;
      }
    case 500:
      {
        sleep_time = WDTO_500MS;
      }
    case 1000:
      {
        sleep_time = WDTO_1S;
      }
    case 2000:
      {
        sleep_time = WDTO_2S;
      }
    case 4000:
      {
        sleep_time = WDTO_4S;
      }
  }
  wdt_enable(sleep_time);
  wdt_reset(); //reset the watchdog timer
#ifdef WDTCSR
  WDTCSR |= _BV(WDIE); //make sure the watchdog timer doesnt trigger a device reset
#else
  WDTCR |= _BV(WDIE);
#endif
  set_sleep_mode(SleepMode);

  // Disable all interrupts
  uint8_t SREGcopy = SREG;
  cli();

#ifdef EIMSK
  uint8_t EIMSKcopy = EIMSK; EIMSK = 0; //disable internal interrupt mask register
#endif
#ifdef PCMSK0
  uint8_t PCMSK0copy = PCMSK0; PCMSK0 = 0; //disable pin change mask registers
#endif
#ifdef PCMSK1
  uint8_t PCMSK1copy = PCMSK1; PCMSK1 = 0;
#endif
#ifdef PCMSK2
  uint8_t PCMSK2copy = PCMSK2; PCMSK2 = 0;
#endif
#ifdef TIMSK0
  uint8_t TIMSK0copy = TIMSK0; TIMSK0 = 0;//disable timer interrupt mask registers
#endif
#ifdef TIMSK1
  uint8_t TIMSK1copy = TIMSK1; TIMSK1 = 0;
#endif
#ifdef TIMSK2
  uint8_t TIMSK2copy = TIMSK2; TIMSK2 = 0;
#endif
#ifdef SPCR
  uint8_t SPCRcopy = SPCR; SPCR &= ~_BV(SPIE); //disable SPI interrupt enable to 0
#endif
#ifdef UCSR0B
  uint8_t UCSR0Bcopy = UCSR0B; UCSR0B &= ~(_BV(RXCIE0) | _BV(TXCIE0) | _BV(UDRIE0)); //RXCIE0: disable complete interrupts. --- TXCIE0: disable transmission complete interrupts --- UDRIE0: disable data register empty interrupts
#endif
#ifdef TWCR
  uint8_t TWCRcopy = TWCR; TWCR &= ~_BV(TWIE); //disable two wire interface interrupt
#endif
#ifdef ACSR
  uint8_t ACSRcopy = ACSR; ACSR &= ~_BV(ACIE); //disable analog comparator interrupt
#endif
#ifdef ADCSRA
  uint8_t ADCSRAcopy = ADCSRA; ADCSRA &= ~_BV(ADIE); //disable adc conversion interrupt
#endif
#if defined(SPMCSR) && defined(SPMIE)
  uint8_t SPMCSRcopy = SPMCSR; SPMCSR &= ~_BV(SPMIE); //disable spm interrupt (store program memory)
#endif

set_sleep_mode(SLEEP_MODE_PWR_DOWN);
sleep_enable();
  sei(); //set the bit and reenable interrupt for watchdog timer
  sleep_mode();            // here the device is actually put to sleep!!
  sleep_disable();
  wdt_disable();           // first thing after waking from sleep: disable watchdog...

  // Reenable all interrupts
#if defined(SPMCSR) && defined(SPMIE)
  SPMCSR = SPMCSRcopy;
#endif
#ifdef ADCSRA
  ADCSRA = ADCSRAcopy;
#endif
#ifdef ACSR
  ACSR = ACSRcopy;
#endif
#ifdef TWCR
  TWCR = TWCRcopy;
#endif
#ifdef UCSR0B
  UCSR0B = UCSR0Bcopy;
#endif
#ifdef SPCR
  SPCR = SPCRcopy;
#endif
#ifdef TIMSK2
  TIMSK2 = TIMSK2copy;
#endif
#ifdef TIMSK1
  TIMSK1 = TIMSK1copy;
#endif
#ifdef TIMSK0
  TIMSK0 = TIMSK0copy;
#endif
#ifdef PCMSK2
  PCMSK2 = PCMSK2copy;
#endif
#ifdef PCMSK1
  PCMSK1 = PCMSK1copy;
#endif
#ifdef PCMSK0
  PCMSK0 = PCMSK0copy;
#endif
#ifdef EIMSK
  EIMSK = EIMSKcopy;
#endif

  SREG = SREGcopy;

#ifdef WDTCSR
  WDTCSR &= ~_BV(WDIE);
#else
  WDTCR &= ~_BV(WDIE);
#endif
}

void FluoLowPower::sleepNowInt(){
  set_sleep_mode (SLEEP_MODE_PWR_DOWN);  
  sleep_enable();

  byte old_ADCSRA = ADCSRA;
  // disable ADC to save power
  ADCSRA = 0;  

  power_all_disable ();  // turn off various modules
   
  PCIFR  |= bit (PCIF0);   // clear any outstanding interrupts
  PCICR  |= bit (PCIE0);   // enable pin change interrupts
  
  sleep_cpu ();  
 
  // cancel sleep as a precaution
  sleep_disable();
  power_all_enable ();   // enable modules again
  ADCSRA = old_ADCSRA;   // re-enable ADC conversion
}
/**
   If enough time has passed since last action, disable unneeded peripherals, calibrate to correct time and sleep.
*/
void FluoLowPower::powerSaveModeOn(uint8_t sleep_t) {

  //shut off extra things
  /*if (DISABLE_TIMER1)
    //Narcoleptic.disableTimer1();
  if (DISABLE_TIMER2)
    //Narcoleptic.disableTimer2();
  if (DISABLE_SERIAL) {
    Serial.end();
   // Narcoleptic.disableSerial();
  }
  if (DISABLE_ADC)
   // Narcoleptic.disableADC();
  if (DISABLE_WIRE)
    //Narcoleptic.disableWire();
  if (DISABLE_SPI)*/
    //Narcoleptic.disableSPI();

  //sleepNow(sleep_t);
  sleepNowInt();
}

void FluoLowPower::powerSaveModeOff() {

}


