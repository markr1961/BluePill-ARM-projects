///
// @brief all the 'junk' needed for a typical project.

#ifndef     _HARDWARE_H
#define     _HARDWARE_H

//
//  Include Files
//
// System Include Files
#include <stdlib.h>       // abs()
#include <stdbool.h>      // bool type defined properly whether using C99 or C++
#include <stdio.h>        // needed for NULL and debug printf.
#include <stddef.h>       // offsetof()
#include <string.h>       // memset, memcpy
// needed for IAR v8.x+:
// #include <intrinsics.h>   // including  __no_operation()

#include "gd32f3x0.h"
// Include files for the GD32F330 peripherals
#include "gd32f3x0_adc.h"
#include "gd32f3x0_crc.h"
#include "gd32f3x0_dbg.h"
#include "gd32f3x0_dma.h"
#include "gd32f3x0_exti.h"
#include "gd32f3x0_fmc.h"
#include "gd32f3x0_fwdgt.h"
#include "gd32f3x0_gpio.h"
#include "gd32f3x0_misc.h"
#include "gd32f3x0_rcu.h"
#include "gd32f3x0_syscfg.h"
#include "gd32f3x0_timer.h"
#include "gd32f3x0_tsi.h"   // touch sensor
#include "gd32f3x0_usart.h"
// USB is in a separate USBFS and USBD library.

// Project Includes
#include  "pindefs.h"
#include  "NVIC.h"
#include  "CycleTime.h"
#include  "PVD.h"
#include  "WDog.h"
#include  "DMA.h"
#include  "ADC.h"
#include  "Timers.h"
#include  "CRCCheck.h"
#include  "Serial.h"
#include  "LED.h"
#include  "debug.h"

//
// Exported Variables
//
extern volatile bool button_pressed;

//  factory programmed unique chip ID registers (96-bits total).
//  See section 1.7.2 of the GD32F3x0 User Manual.
#define     U_ID_31_00_ADDRESS          ((__I uint32_t *) 0x1FFFF7AC)
#define     U_ID_63_32_ADDRESS          ((__I uint32_t *) 0x1FFFF7B0)
#define     U_ID_95_64_ADDRESS          ((__I uint32_t *) 0x1FFFF7B4)

#define     U_ID_31_00                  (*U_ID_31_00_ADDRESS)
#define     U_ID_63_32                  (*U_ID_63_32_ADDRESS)
#define     U_ID_95_64                  (*U_ID_95_64_ADDRESS)

//
//  EXPORTED FUNCTIONS
//
void InitGPIO(void);
void InitHardware(void);
#endif
