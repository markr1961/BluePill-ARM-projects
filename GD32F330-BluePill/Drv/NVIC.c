///
// @brief  
//
// Pragmas and Conditional Control
//
#pragma  section = "ExceptionInfo"

//
// Include Files
//
#include "Hardware.h"

#define JUMP_FAULT_LOGGER()   \
  __asm volatile (            \
  "tst LR, #4       \n"       \
  "ite EQ           \n"       \
  "mrseq R0,MSP     \n"       \
  "mrsne R0, PSP    \n"       \
  "mov   R1, R4     ; Registers R4-R6, as parameters 2-4 of the function called\n"  \
  "mov   R2, R5     \n"       \
  "mov   R3, R6     \n"       \
  "b hard_fault_logger_c \n"  \
  );

//
// Type Definitions
//
typedef struct
{
   uint8_t             u8Flag;   // For RCC_GetFlagStatus()
   reset_event_code_en enReason;
}  reset_st;

/***
 * NOTE: If this var grows to >100 bytes, the EXCEPTION block in the
 * linker file also needs to grow also.
 */
arm32_excep_reg_st    stExReg  @ "ExceptionInfo";
uint32_t *addr_source;

///
// @brief log the registers to exception struct
//
void hard_fault_logger_c (unsigned int * hardfault_args)
{
  stExReg.u32R0 = (unsigned long) hardfault_args[0];
  stExReg.u32R1 = (unsigned long) hardfault_args[1];
  stExReg.u32R2 = (unsigned long) hardfault_args[2];;
  stExReg.u32R3 = (unsigned long) hardfault_args[3];
  stExReg.u32R12 = (unsigned long) hardfault_args[4];
  stExReg.u32LR = (unsigned long) hardfault_args[5];
  stExReg.pPC =   (uint32_t*) hardfault_args[6];
  stExReg.u32PSR = (unsigned long) hardfault_args[7];
  stExReg.u32BFAR = (*((volatile unsigned long *)(0xE000ED38)));
  stExReg.u32CFSR = (*((volatile unsigned long *)(0xE000ED28)));
  stExReg.u32HFSR = (*((volatile unsigned long *)(0xE000ED2C)));
  stExReg.u32DFSR = (*((volatile unsigned long *)(0xE000ED30)));
  stExReg.u32AFSR = (*((volatile unsigned long *)(0xE000ED3C)));
  stExReg.u32SCB = SCB->SHCSR;

  stExReg.ExceptSysTick = SyctickCounter;
  stExReg.ExceptCount++; // number of exceptions
  stExReg.signature = 0xA5;

  //  stExReg.enExceptType = ; // <- need something here.

  DEBUG_BREAK();
  
  exit(SUCCESS);  // for now.
  
}

///
// @brief   use the NVIC to generate a software reset of the chip.
// @param   exit source
// @return none
void exit(int arg)
{
   //We're shutting down, so prevent any interrupts that might prevent this
  __disable_interrupt();
  // Software Reset
#ifdef DEBUG
  // Since we are using the watchdog for a reset, unfreeze it here in debug mode
  dbg_periph_enable(DBG_FWDGT_HOLD);
#endif
  NVIC_SystemReset();
}

///
// @brief initializes the NVIC and interrupts
// @param none
// @return none
void InitNVIC(void)
{
  stExReg.enResetReason = enGetResetReason();
  stExReg.PreviousSysTick = stExReg.LastSysTick;
  
  // Configure the NVIC for 16 levels of preemption priority
  // Priority is 0-15, 0 being highest
  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  // Set the priority of the other interrupts.
  // The priority level is first encoded and the returned encoded priority is
  // used to set the priority.
  //  NVIC_EncodePriority(PriorityGroup, PreemptPriority, SubPriority)
  //    PriorityGroup     Used priority group
  //    PreemptPriority   Preemptive priority value (starting from 0)
  //    SubPriority       Sub-priority value (starting from 0)
  //    Return:           Encoded priority level to be used in NVIC_SetPriority()

}

///
// @brief returns the reset reason from the RCU.
// @param none
// @return enReason - reason for reset
//
reset_event_code_en enGetResetReason(void)
{
  reset_event_code_en enReason;

  if (rcu_flag_get(RCU_FLAG_FWDGTRST))
    enReason = ARM_WATCHDOG_RESET;     // Independent Watchdog
  else if (rcu_flag_get(RCU_FLAG_WWDGTRST))
    enReason = ARM_WATCHDOG_RESET;     // Window Watchdog
  else if (rcu_flag_get(RCU_FLAG_SWRST))
    enReason = ARM_SOFTWARE_RESET;     // Self (software)
  else if (rcu_flag_get(RCU_FLAG_PORRST))
    enReason = ARM_POWER_ON_RESET;     // Power-on
  else if (rcu_flag_get(RCU_FLAG_EPRST))
    enReason = ARM_EXTERNAL_RESET;     // External pin reset
  else if (rcu_flag_get(RCU_FLAG_LPRST))
    enReason = ARM_LOW_POWER_RESET;    // Low power
  else if (rcu_flag_get(RCU_FLAG_OBLRST))
    enReason = ARM_OPTION_BYTE_RESET;  // Option byte reset
  else if (rcu_flag_get(RCU_FLAG_V12RST))
    enReason = ARM_OPTION_BYTE_RESET;  // V12 domain power reset
  else
    enReason = ARM_UNKNOWN_RESET;     // Unknown reset

  rcu_all_reset_flag_clear();

#ifdef FAULT_MSG
  printf("reset=%d\n",enReason);
#endif
  return(enReason);
}

///
// @brief BusFault Handler
//
void BusFault_Handler(void)
{
  DEBUG_BREAK();
  
	JUMP_FAULT_LOGGER();

  exit(EXIT_BUS_FAULT);
}

///
// @brief HardFault_Handler
//
void HardFault_Handler(void)
{
   // From 'STM32F10xxx Cortex-M3 programming manual.pdf'.
   // "A hard fault is an exception that occurs because of an error during
   // exception processing, or because an exception cannot be managed by
   // any other exception mechanism."
  DEBUG_BREAK();
  
//  addr_source = (uint32_t*)&HardFault_Handler;

	JUMP_FAULT_LOGGER();

  exit(EXIT_HARD_FAULT);
}

///
// @brief NMI_Handler
#pragma optimize=low
void NMI_Handler(void)
{
  DEBUG_BREAK();

	JUMP_FAULT_LOGGER();

  exit(EXIT_NMI);
}

void UsageFault_Handler(void)
{
   // From 'STM32F10xxx Cortex-M3 programming manual.pdf'.
   // "A usage fault is an exception that occurs because of a fault related
   // to instruction execution. This includes:
   //      * An undefined instruction
   //      * An illegal unaligned access
   //      * Invalid state on instruction execution
   //      * An error on exception return.
   // The following can cause a usage fault when the core is configured to
   // report them:
   //      * An unaligned address on word and halfword memory access
   //      * Division by zero
  DEBUG_BREAK();

	JUMP_FAULT_LOGGER();

  exit(EXIT_USAGE_FAULT);
}

/*!
    \brief      this function handles MemManage exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void MemManage_Handler(void)
{
  DEBUG_BREAK();
  JUMP_FAULT_LOGGER();

  exit(EXIT_MEM_FAULT);
}

/*!
    \brief      this function handles SVC exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SVC_Handler(void)
{
  DEBUG_BREAK();
}

/*!
    \brief      this function handles DebugMon exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DebugMon_Handler(void)
{
  DEBUG_BREAK();
}

/*!
    \brief      this function handles PendSV exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void PendSV_Handler(void)
{
  DEBUG_BREAK();
}
