//
// Include Files
//
#include "Hardware.h"

//
// External Variable Definitions
//
//

//
// Local Variable Definitions
//

void InitPVD(void)
{

  //Enable the Clock for the Power Module
  rcu_periph_clock_enable(RCU_PMU);

  //PWR_PVDLevelConfig(PWR_PVDLevel_2V9);
  pmu_lvd_select(PMU_LVDT_5);

  /* configure NVIC */
  nvic_priority_group_set(NVIC_PRIGROUP_PRE0_SUB4);
  nvic_irq_enable(LVD_IRQn,2,1);

  //Initialize the External Interrupt Structures
  exti_interrupt_flag_clear(EXTI_16);
  /* configure EXTI_16  */
  exti_init(EXTI_16, EXTI_INTERRUPT, EXTI_TRIG_BOTH);
}

void LVD_IRQHandler(void)
{
#ifdef DEBUG
  printf("LVD TRIP\r\n");
#endif
  // Then wait here forever.
  while(true)
    ;
}
