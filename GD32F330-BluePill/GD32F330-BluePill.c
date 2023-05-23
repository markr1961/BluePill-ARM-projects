///
// @brief demo project for GD32F330C8 on Blue Pill board.
//
// Cortex-M 20 pin connector
//  VRef  1   2   NC  
//        3   4   GND  
//        5   6   GND  
//  SWDIO 7   8   GND  
//  SWCLK 9  10   GND  
//        11 12   GND  
//  SWO   13 14   NC  
//  reset 15 16   NC  
//        17 18   NC  
//  5V    19 20   NC
//
// debug connector
// 3.3V  
// SWDIO/PA13  pin 34
// SWCLK/PA14  pin 37
// GND 
// also need reset (mislabeled "B2")

//
// Include Files
//
#include "Hardware.h"

//
// Local Function Prototypes
//
static void SetSystemClocks(void);

__weak void UpdateTime(void) {}; // create sometime later.

//
// Local Variable Definitions
//
static led_state_en LedState = LED_ON;
static uint32_t LastTime = 0;
#ifdef DEBUG
bool button_state;
#endif

void main(void)
{
  // NOTE: default clock and NVIC offset are set in SystemInit() in system_gd32f3x0.c
  // The default clock is __SYSTEM_CLOCK_8M_IRC8M while the NVIC offset is VECT_TAB_OFFSET
  SetSystemClocks();    // set the clocks
  InitWatchdog();
  InitNVIC();
  StartSysTick();
  InitHardware();
  
  PatWatchdog();

  while(true)
  {
    GenerateADC();

    UpdateTime();
    if (SyctickCounter > LastTime + (CYCLES_PER_SECOND * MILLISECONDS_PER_LOOP)/2)
    {
      LedState = (led_state_en) !LedState;
      ControlLED(LedState);   // update the LED
      LastTime = SyctickCounter;
    }
#ifdef DEBUG
    button_state = gpio_input_bit_get(BUTTON_PORT, BUTTON_PIN);
#endif
    if (button_pressed)
    {
#ifdef DEBUG
      printf("Vcc=%dmV temp=%dc Vbat=%dmV\n",u16GetVccRead(), s8GetTempRead(), u16GetVbatRead());
      printf("Joystick-1 %d, Joystick-2 %d\n", readJoystick(JOYSTICK_X_INDEX),readJoystick(JOYSTICK_Y_INDEX));
      
      // create a hard fault:
      // SCB->CCR |= SCB_CCR_DIV_0_TRP_Msk;
      // DEBUG_NOP();
      // button_state = button_state/button_state;
#endif
      button_pressed = false;
    }
    //delay_ms(5);
    PatWatchdog();  // Pat the dog

    WaitForCycle();
  }
}

/*!
    \brief      configure the system clock to 12MHz by PLL which selects IRC8M/2 as its clock source
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void SetSystemClocks(void)
{
#ifdef DEBUG
  // get the core clock before the pll is configured
  SystemCoreClockUpdate();
#endif

  /* PLL = (IRC8M/2) * 3 = 12 MHz */
  rcu_pll_config(RCU_PLLSRC_IRC8M_DIV2, RCU_PLL_MUL3);

  // Set the AHB pre-scaler for no divide so System and CPU clocks are the same
  rcu_ahb_clock_config(RCU_AHB_CKSYS_DIV1);

  // Set the APB1 Clock to 6 MHz
  rcu_apb1_clock_config(RCU_APB1_CKAHB_DIV2);

  // Set the APB2 Clock to 6 MHz
  rcu_apb2_clock_config(RCU_APB2_CKAHB_DIV2);

  /* enable PLL */
  RCU_CTL0 |= RCU_CTL0_PLLEN;

  // Wait until the PLL is ready.
  rcu_osci_stab_wait(RCU_PLL_CK);

  /* select PLL as system clock */
  rcu_system_clock_source_config(RCU_CKSYSSRC_PLL);

  /* wait until PLL is selected as system clock */
  while(0U == (RCU_CFG0 & RCU_SCSS_PLL))
    ; // yes this will hang the unit if the clocks and PLL don't work, but if they don't it's hosed anyway.

  // get the core clock now that the pll is configured
  SystemCoreClockUpdate();
  DEBUG_NOP();
}
