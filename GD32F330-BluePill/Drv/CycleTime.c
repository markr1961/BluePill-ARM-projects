///
// @brief variables and functions to manage a consistent cycle time.

//
// Include Files
//
#include "Hardware.h"

//
// Type Definitions
//
typedef struct MasterClocks
{
  uint32_t SYSCLK_Frequency;  /*!< returns SYSCLK clock frequency expressed in Hz */
  uint32_t HCLK_Frequency;    /*!< returns HCLK clock frequency expressed in Hz */
  uint32_t PCLK1_Frequency;   /*!< returns PCLK1 clock frequency expressed in Hz */
  uint32_t PCLK2_Frequency;   /*!< returns PCLK2 clock frequency expressed in Hz */
  uint32_t ADCCLK_Frequency;  /*!< returns ADCCLK clock frequency expressed in Hz */
} RCU_ClocksTypeDef;

//
// Global Variable Definitions
//
volatile uint32_t  SyctickCounter;

//
// Local Variable Definitions
//
static RCU_ClocksTypeDef   stSystemClock;
static volatile uint8_t    u8SysTicks;       // counts 1mS systicks between overrun checks
static volatile bool       bCycleFlag;
static uint32_t     u32CycleTime, u32StartTime, u32EndTime, u32MaxCycleTime, u32MinCycleTime;
//
// Local Function Prototype Declarations
//

///
// @brief read and save the various system clock.
// @pararm none
// @return none
void GetClockFreq(void)
{
  stSystemClock.SYSCLK_Frequency = rcu_clock_freq_get(CK_SYS);
  stSystemClock.HCLK_Frequency = rcu_clock_freq_get(CK_AHB);
  stSystemClock.PCLK1_Frequency = rcu_clock_freq_get(CK_APB1);
  stSystemClock.PCLK2_Frequency = rcu_clock_freq_get(CK_APB2);
  stSystemClock.ADCCLK_Frequency = rcu_clock_freq_get(CK_ADC);
}

///
// @brief calls the command that puts the core to sleep
static void Sleep(void)
{
#ifndef DEBUG
//   __WFI();   // go to sleep
  __WFE();   // Wait for event
#else
  __no_operation();
#endif
}

///
// @brief delays the specified number of milliseconds.
// @param ms - The number of milliseconds
// @return none
// @note !!! BLOCKING !!!
void delay_ms(uint32_t ms)
{
  uint32_t last_systick = SyctickCounter;
  while (SyctickCounter <= last_systick + ms)
    Sleep();
}

///
// @brief init the loop timing vars and start the SysTick
//
void StartSysTick(void)
{
  // Initialize the loop timing variables
  u32MaxCycleTime = 0x00000000;
  u32MinCycleTime = 0xFFFFFFFF;
  bCycleFlag      = false;
  u32StartTime    = 0x00000000;

  // Set up the SysTick for a 1mS rollover
  SysTick_Config(SystemCoreClock/1000);

  // should be running at 12.0MHz. Calculate and save the resultant system
  // clock frequencies for reference by other modules.
  GetClockFreq();

  // Configure NVIC for SysTick interrupts.
  NVIC_SetPriority(SysTick_IRQn, 1);
  NVIC_EnableIRQ(SysTick_IRQn);

#ifdef PRINT_CLOCKS
  printf("System Core clock = %d\r\n", SystemCoreClock);
  printf("SysClock = %d\r\n", stSystemClock.SYSCLK_Frequency);
  printf("HCLK = %d\r\n", stSystemClock.HCLK_Frequency);
  printf("PCLK1 = %d\r\n", stSystemClock.PCLK1_Frequency);
  printf("PCLK2 = %d\r\n", stSystemClock.PCLK2_Frequency);
  printf("ADC clock = %d\r\n", stSystemClock.ADCCLK_Frequency);
#endif

  // _NOW_ enable the SysTick interrupt and start the clock.  The act of enabling also
  // takes care of the initial load to the value in SysTick->LOAD.
  SysTick->CTRL |= (SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
}

///
// @brief SysTick_Handler. Clears the interrupt and sets the bCycleFlag.////////
void SysTick_Handler(void)
{
  SyctickCounter++;
  u8SysTicks++;

  //Clear the Interrupt Count Flag
  SysTick->CTRL &= ~SysTick_CTRL_COUNTFLAG_Msk;

  if (SyctickCounter%SYSTICKS_PER_LOOP == 0)
  {
    //Set the Cycle Time Flag
    bCycleFlag = true;
  }
  stExReg.LastSysTick = SyctickCounter;
}

///@breif read the current SysTick counter.
uint32_t u32GetTime(void)
{
  return(SysTick->VAL);
}

///
// @brief pauses the main loop, calling Sleep() until bCycleFlag is set
//
void WaitForCycle(void)
{
  if(bCycleFlag)
  {
    // busted the loop
    DEBUG_BREAK();
    // do some calculations
  }

  u32EndTime = SyctickCounter;

  while(bCycleFlag == false)
  {
    Sleep();
  }
  // set up for the next cycle
  bCycleFlag = false;
  // calculate the cycle time:
  u32CycleTime = u32EndTime - u32StartTime;
  // save min/max:
  if (u32CycleTime < u32MinCycleTime)
    u32MinCycleTime = u32CycleTime;
  if (u32CycleTime > u32MaxCycleTime)
    u32MaxCycleTime = u32CycleTime;
  // now update the start of loop time
  u32StartTime = SyctickCounter;

}

///
// @brief   return Pclk for timers
// @param   TIMER
// @return  u32 clock in Hz.
// If the APBx Pre-scaler is 1, timers run at APBx Clk, in any other case, 
// they run at x2 the APBx Clk
uint32_t u32GetTimerClock(uint32_t timer)
{
  uint32_t clock = 0;
  switch(timer)
  {
  case TIMER1:
  case TIMER2:
  case TIMER13:
    if ((RCU_CFG0 & RCU_CFG0_APB1PSC) == RCU_APB1_CKAHB_DIV1)
      clock = stSystemClock.PCLK1_Frequency;
    else 
      clock = 2 * stSystemClock.PCLK1_Frequency;
    break;
  case TIMER0:
  case TIMER14:
  case TIMER15:
  case TIMER16:
    if ((RCU_CFG0 & RCU_CFG0_APB2PSC) == RCU_APB2_CKAHB_DIV1)
      clock = stSystemClock.PCLK2_Frequency;
    else 
      clock = 2 * stSystemClock.PCLK2_Frequency;
    break;
  default:
    DEBUG_BREAK();
    clock = 0;
    break;
  }
  return(clock);
}
