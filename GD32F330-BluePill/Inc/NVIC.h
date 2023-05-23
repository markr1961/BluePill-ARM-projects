#ifndef _NVIC_H
#define _NVIC_H

//
//  INCLUDE FILES
//
#include "ICF_Symbols.h"

//
// EXPORTED MACRO CONSTANTS
//
//  Non-ANSI Standard C codes for exit(). These are used by the exception handlers to log the reset reason.
#define     EXIT_NMI                    2
#define     EXIT_HARD_FAULT             3
#define     EXIT_BUS_FAULT              4
#define     EXIT_USAGE_FAULT            5
#define     EXIT_MEM_FAULT              6

#ifndef NVIC_PRIORITYGROUP_0
  #define NVIC_PRIORITYGROUP_0         ((uint32_t)0x00000007) /*!< 0 bit  for pre-emption priority,
                                                                   4 bits for subpriority */
  #define NVIC_PRIORITYGROUP_1         ((uint32_t)0x00000006) /*!< 1 bit  for pre-emption priority,
                                                                   3 bits for subpriority */
  #define NVIC_PRIORITYGROUP_2         ((uint32_t)0x00000005) /*!< 2 bits for pre-emption priority,
                                                                   2 bits for subpriority */
  #define NVIC_PRIORITYGROUP_3         ((uint32_t)0x00000004) /*!< 3 bits for pre-emption priority,
                                                                   1 bit  for subpriority */
  #define NVIC_PRIORITYGROUP_4         ((uint32_t)0x00000003) /*!< 4 bits for pre-emption priority,
                                                                   0 bit  for subpriority */
#endif

//
// EXPORTED MACRO FUNCTIONS
//

//
// EXPORTED TYPES
//
/* GD32F3x0 reset cause event definitions. */
typedef enum
{
   ARM_POWER_ON_RESET,
   ARM_EXTERNAL_RESET,
   ARM_SOFTWARE_RESET,
   ARM_LOW_POWER_RESET,
   ARM_WATCHDOG_RESET,
   ARM_OPTION_BYTE_RESET,
   ARM_UNKNOWN_RESET
}  reset_event_code_en;

typedef enum
{
  NO_EXCEPTION,       //0
  ASSERT_EXCEPTION,   //1
  BUS_EXCEPTION,      //2
  HARD_EXCEPTION,     //3
  MEM_EXCEPTION,      //4
  NMI_EXCEPTION,      //5
  USE_EXCEPTION,      //6
  OTHER_EXCEPTION     //7
} exception_type_en;

typedef struct
{
  // ARM32 System Control Block registers.
  uint32_t u32HFSR;
  uint32_t u32CFSR;  // Includes UFSR, BFSR & MMFSR
  uint32_t u32MFAR;  // Only valid if MMARVALID=1
  uint32_t u32BFAR;  // Only valid if BFARVALID=1
  // SP value
  uint32_t *pSP;
  // Stacked registers.
  uint32_t u32R0;
  uint32_t u32R1;
  uint32_t u32R2;
  uint32_t u32R3;
  uint32_t u32R12;
  uint32_t u32LR;
  uint32_t *pPC;
  uint32_t u32PSR;
  uint32_t u32SCB;
  uint32_t u32DFSR;
  uint32_t u32AFSR;
  // NX project vars:
  exception_type_en enExceptType;
  
  uint32_t ExceptSysTick;   // 1mS counter at time of exception
  uint32_t ExceptCount;     // number of exceptions since last cleared
  uint32_t LastSysTick;     // continuously updated from 1mS counter. Useful for catching WDs
  uint32_t PreviousSysTick; // copied from Last SysTick on startup
   reset_event_code_en enResetReason;
  /* !!! keep as last entry: !!! */
  uint8_t signature;
} arm32_excep_reg_st;

//
// EXPORTED VARIABLES
//
extern arm32_excep_reg_st    stExReg;
//
// EXPORTED FUNCTIONS
//
reset_event_code_en enGetResetReason(void);
void InitNVIC(void);
void exit(int arg);
#endif
