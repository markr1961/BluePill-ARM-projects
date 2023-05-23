## Blue Pill related projects and info

[https://github.com/markr1961/BluePill-ARM-projects](https://github.com/markr1961/BluePill-ARM-projects)

### Projects
* [GD32F330-BluePill](GD32F330-BluePill/readme.md): Blue Pill board with GigaDevices GD32F330C8T6

#### debug connector
3.3V  
SWDIO/PA13  
SWCLK/PA14  
GND  

#### Boot block
Boot0: 1 to enter bootloader  
Boot1/PB2  
reset: mislabeled 'B2'  

#### USART1 (bootloader UART)
RX PA10  
TX PA9  

#### LED
PC13  

#### Xtal
8MHz  PF0/PF1 (not brought to pins)  
32KHz PC14/PC15  

#### USB
D+  PA12 (pull down)  
D-  PA11  

### Cortex-M 20 pin connector
  VRef  1   2   NC  
        3   4   GND  
        5   6   GND  
  SWDIO 7   8   GND  
  SWCLK 9  10   GND  
        11 12   GND  
  SWO   13 14   NC  
  reset 15 16   NC  
        17 18   NC  
  5V    19 20   NC  
- [Seeger J-link User Guide](https://wiki.segger.com/UM08001_J-Link_/_J-Trace_User_Guide)

## Blue Pill Resources
- [STM32 First steps](https://medium.com/@iottestlora/stm32-first-steps-1143dca5a275) picture, pinout schematic
- [Stm32 Bluepill using the Arduino SimpleFOCShield](https://docs.simplefoc.com/bluepill_connection)
- [Introduction to the STM32 Blue Pill (STM32duino)](https://predictabledesigns.com/introduction-stm32-blue-pill-stm32duino/)
- [Getting Started with STM32 (Blue Pill) using Arduino IDE: Blinking LED](https://circuitdigest.com/microcontroller-projects/getting-started-with-stm32-blue-pill-development-board-stm32f103c8-using-arduino-ide) 
  uses FTDI and Boot0 loader

## STM32 Projects
- [Buck50 STM32F103 logic analyzer and more](https://awesomeopensource.com/project/thanks4opensource/buck50)
