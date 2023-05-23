//
// Include Files
//
// System Include Files
#include "Hardware.h"
#include "pindefs.h"

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/

void InitGPIO(void)
{
  /* GPIO Ports Clock Enable */
//  rcu_periph_clock_enable(RCU_GPIOA);
  rcu_periph_clock_enable(RCU_GPIOB);
  rcu_periph_clock_enable(RCU_GPIOC);
//  rcu_periph_clock_enable(RCU_GPIOF);
  rcu_periph_clock_enable(RCU_CFGCMP);

  /* 
    Set pins that need to start in a specific state: 
  */
  gpio_bit_reset(LED_PORT, LED_PIN);

  /* LED Output */
  gpio_mode_set(LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_PIN);
  gpio_output_options_set(LED_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_2MHZ, LED_PIN);

  /* configure button pin as input */
  gpio_mode_set(BUTTON_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, BUTTON_PIN);

  /* configure the priority group */
  nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
  /* enable and set key user EXTI interrupt to the lower priority */
  nvic_irq_enable(EXTI4_15_IRQn, 2U, 1U);

  /* connect key user EXTI line to key GPIO pin */
  syscfg_exti_line_config(EXTI_SOURCE_GPIOB, EXTI_SOURCE_PIN12);

  /* configure key user EXTI line */
  exti_init(EXTI_12, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
  exti_interrupt_flag_clear(EXTI_12);
}
