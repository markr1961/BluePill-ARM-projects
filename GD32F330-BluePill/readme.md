## GD32F330-BluePill development project
GigaDevices GD32F330C8T6 on a Blue Pill dev board, purchased from some random seller on AliExpress

### CPU
[GigaDevices GD32F330C8T6](https://www.gigadevice.com/product/mcu/arm-cortex-m4/gd32f330c8t6)  
Cortex-M4 at 84MHz max.  
64K flash, 8K RAM, LQFP48  

## Build Tools
### IAR v7.80.4
NOTE: There is a tool to add the GD32F3x0 devices to IAR 7.x or 8.x in the folder .\\GD32F3x0_AddOn  
      run "IAR_GD32F3x0_ADDON_2.0.0.exe" _once_ to add GD F3x0 series to the list of devices recognized by IAR  
      It should find the correct IAR version location; which for 7.8 is 7.5 and for 8.4 is  8.3  

### Libraries and system init
using GD32F3x0_Firmware_Library_V2.2.1. Libraries are built as part of the project.  
The default clock and NVIC offset are set in SystemInit() in system_gd32f3x0.c  
The default clock is __SYSTEM_CLOCK_8M_IRC8M while the NVIC offset is VECT_TAB_OFFSET  
The default CPU include file, gd32f3x0.h defines an enum for bool. This must be commented out when using stbbool  

#### include paths
\$PROJ\_DIR\$\\GD32_Include  
\$PROJ\_DIR\$\\GD32F3x0_standard_peripheral\\Include  
\$PROJ\_DIR\$\\GD32F3x0_standard_peripheral\\CMSIS\\GD\\GD32F3x0\\Include  
\$PROJ\_DIR\$\\GD32F3x0_standard_peripheral\\CMSIS  

## debugger
It works with an ST-link V2. What can I say? ¯\_(ツ)_/¯  

### System Resources
#### Clocks
System Core  12,000,000  
SysClock 12,000,000  
HCLK 12,000,000  
PCLK1 6,000,000  
PCLK2 6,000,000  
ADC clock 1,000,000  
SysTick 1mS
#### Timer16
12MHz input, 3 prescaler for 4KHz PWM with 1,000 counts.

## --- commit notes ---
05/17/2023
initial commit. main loop runs @ 10mS loop, LED blinks @1Hz.  

05/18/2023
ADC with DMA sampling PA1/PA2 inputs plus Vref, Vbat and Temp channels.  
PB12 generating user button interrupt. (it really needs a debouncer!)  
clean up spurious files.  
