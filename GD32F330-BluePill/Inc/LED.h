//
// MODULE DECLARATION
//
#ifndef     _LEDS_H
#define     _LEDS_H

//
// Include Files
//
#include "Hardware.h"

//
// Exported Types
//
  
  typedef enum
  {
    NO_BLINKS,
    ONE_BLINK,
    TWO_BLINKS,
    THREE_BLINKS,
    FOUR_BLINKS
  } led_command_en;
  
  typedef enum
  {
    LED_OFF,
    LED_ON
  } led_state_en;

  typedef struct
  {
    uint16_t       u16BlinkRate;
    led_state_en   enState;
    led_command_en enPattern;
  } led_parms_st;

//
//  EXPORTED VARIABLES
//

//
//  EXPORTED FUNCTIONS
//
void ControlLED(led_state_en enState);

#endif
