//
// INCLUDE FILES
//
#include "Hardware.h"
#include "pindefs.h"

void InitLEDDrv(void)
{
  // Turn the LED off
  // LED I/O pin is set up in GPIO.c/GPIO_Init()
  ControlLED(LED_OFF);
}

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:     ControlLED
//  DESCRIPTION:  This function sets the Red LED to the desired state.
//                LED drive is low active
//  PARAMS:       enState (LED_ON or LED_OFF)
//  RETURN:       none
//  AUTHOR:       Eric Weber
//  HISTORY:      05/10/2021 - Port from Wasp2 code base for F410 - EW
////////////////////////////////////////////////////////////////////////////////
void ControlLED(led_state_en enState)
{
  if (enState == LED_ON)
  {
    gpio_bit_reset(RED_LED);
  }
  else
  {
    gpio_bit_set(RED_LED);
  }
}
