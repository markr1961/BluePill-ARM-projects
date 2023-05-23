/*
  On the GigaDevices GD32F330Cx, PB9's alternate function is Timer16, CH0.
*/

//
// Include Files
//
#include "Hardware.h"

// Timer clock constants
//
/*
  For a 4KHz PWM output with a timer count of 1000 requires a 4MHz input.
  The APB bus is running at 6MHz for a timer input of 12MHz.
  Therefore a prescaler of 3 is required. 
*/
#define PRESCALE_DIVIDER     3
#define CLOCK_DIVIDER     1000      // Clock divider for PWM at 4KHz frequency (0.333uS/tick)

// operating vars:
static uint16_t pwm_DutyCycle = 800;   // Duty cycle in 1/0th of percent. 0-1000

///
// @brief Initialize the GD32 TIMER16 for use as a PWM output.
//
void InitTimerPwm(void)
{
  timer_parameter_struct    TIM_TimeBaseStructure;
  timer_oc_parameter_struct TIM_OCInitStructure;

  /* PWM from TIMER16/CH0 on PB9 */

  // Turn on peripherals we need, GPIOB
  rcu_periph_clock_enable(RCU_GPIOB); // | RCC_APB2Periph_AFIO);
  // enable the clock to the SysCfg block so the AF source can be set can be set:
  rcu_periph_clock_enable(RCU_CFGCMP);
  
  gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_9);
  gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);

  // use Pin9 alternate function remapping to TIMER4
  gpio_af_set(GPIOB, GPIO_AF_2, GPIO_PIN_9);

  // Enable Timer16 clock and reset to to defaults (also releases reset)
  rcu_periph_clock_enable(RCU_TIMER16);
  timer_deinit(TIMER16);
  timer_internal_clock_config(TIMER16);

  // Timer Base configuration
  timer_struct_para_init(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.prescaler = PRESCALE_DIVIDER-1;
  TIM_TimeBaseStructure.counterdirection = TIMER_COUNTER_UP;
  TIM_TimeBaseStructure.period = CLOCK_DIVIDER;
  TIM_TimeBaseStructure.clockdivision = TIMER_CKDIV_DIV1;
  TIM_TimeBaseStructure.repetitioncounter = 0;

  timer_init(TIMER16, &TIM_TimeBaseStructure);

  // CH0 Configuration in PWM mode.  We're using TIMER16 CH0,
  // which has been mapped to output the PWM pulse on PB9.
  timer_channel_output_struct_para_init(&TIM_OCInitStructure);

  TIM_OCInitStructure.outputstate  = TIMER_CCX_ENABLE;
  TIM_OCInitStructure.outputnstate = TIMER_CCXN_DISABLE;
  TIM_OCInitStructure.ocpolarity   = TIMER_OC_POLARITY_HIGH;
  //TIM_OCInitStructure.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;
  TIM_OCInitStructure.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
  //TIM_OCInitStructure.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;

  timer_channel_output_config(TIMER16, TIMER_CH_0, &TIM_OCInitStructure);

  timer_channel_output_mode_config(TIMER16, TIMER_CH_0, TIMER_OC_MODE_PWM0);
  timer_channel_output_pulse_value_config(TIMER16, TIMER_CH_0, pwm_DutyCycle);

  timer_primary_output_config(TIMER16, ENABLE);

  timer_counter_value_config(TIMER16, 0);

  timer_channel_output_shadow_config(TIMER16, TIMER_CH_0, TIMER_OC_SHADOW_ENABLE);

  /* auto-reload preload enable */
  timer_auto_reload_shadow_enable(TIMER16);

  // // Configure NVIC for Timer 16 interrupts - priority 4
  // //NVIC_SetPriority(TIMER16_IRQn, NVIC_EncodePriority(NVIC_PRIORITYGROUP_4, 2, 0));
  // NVIC_SetPriority(TIMER16_IRQn, 4);
  // NVIC_EnableIRQ(TIMER16_IRQn);
}

///
// @brief   Set duty cycle of PWM output
// @param   nPercent, in integer percent 0-100
void SetPwmDutyCycle(int nPercent)
{
  //uint16_t pulse = 0;

  // Sanity check
  if((nPercent > 100) || (nPercent < 0))
  {
    DEBUG_BREAK();
    return; 
  }

  timer_channel_output_pulse_value_config(TIMER16, TIMER_CH_0, (uint16_t)nPercent*10);

}

///
// @brief return duty cycle of the PWM
// @return PWM output in percent (0-100)
int GetPwmDutyCycle(void)
{
  uint16_t pwmCount = TIMER_CH0CV(TIMER16);
  return ((int)pwmCount/10);
}

void PWM_On(void)
{
  timer_enable(TIMER16);

  // Turn on CH0 output on TIM16
  timer_primary_output_config(TIMER16, ENABLE);

  timer_channel_output_state_config(TIMER16, TIMER_CH_0, TIMER_CCX_ENABLE);

}

void PWM_Off()
{
  // Turn off CH0 outputs on TIMER16

  //TIM4->CCER &= 0x00fff;    // Brute force - STM Timer APIU doesn't work
  timer_channel_output_state_config(TIMER16, TIMER_CH_0, TIMER_CCX_DISABLE);

  timer_primary_output_config(TIMER16, DISABLE);

  timer_disable(TIMER16);

}
