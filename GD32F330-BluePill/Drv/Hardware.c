
//
//
//
#include "Hardware.h"

/***
  * Init init GPIO, DAM, ADC and serial port.
  * Each function's IRQ is initied in that function.
  */
void InitHardware(void)
{
  InitGPIO();
  InitDMA();
  InitADC();
  InitSerial();
#ifdef DEBUG
  dbg_low_power_enable(DBG_LOW_POWER_SLEEP);
  dbg_periph_enable(DBG_TIMER16_HOLD);  // free running timer
#endif
}
