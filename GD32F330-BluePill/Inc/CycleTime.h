///
// @brief exported functions and vars related to cycle time management
#ifndef     _CYCLETIME_H
#define     _CYCLETIME_H

#include  <stdbool.h>
#include  "delay_uSec.h"
//
// Macro Constant Definitions
//
#define CLOCK_FREQUENCY         12000000                    // System clock frequency is 12 MHz
#define SYSTICKS_PER_LOOP             10                    // SysTicks and mSec per super loop
#define MILLISECONDS_PER_LOOP    SYSTICKS_PER_LOOP          // 1 systick = 1 msec
#define CLOCKS_PER_CYCLE       (CLOCK_FREQUENCY/1000)       // # of processor clocks in 1 _systick_)

#define CYCLES_PER_SECOND      (1000/SYSTICKS_PER_LOOP)     // # of super loops in 1 second
#define CYCLES_PER_MINUTE      (CYCLES_PER_SECOND * 60)     // # of super loops in 1 minute
#define CYCLES_PER_HOUR        (CYCLES_PER_MINUTE * 60)     // # of super loops in 1 hour

#define DelayUsec(n)  delay_uSec(n)


//
// Global Variable Definitions
//
extern volatile uint32_t SyctickCounter;

//
// GLOBAL FUNCTION DEFINITIONS
//
void StartSysTick(void);
void WaitForCycle(void);
void GetClockFreq(void);
void delay_ms(uint32_t ms);
uint32_t u32GetTime(void);
uint32_t u32GetTimerClock(uint32_t timer);

#endif // _CYCLETIME_H
