
#ifndef     _PINDEFS_H
#define     _PINDEFS_H

//
// PRAGMA'S AND CONDITIONAL CONTROL
//
#define     DEBUG_UART_NUMBER           0   // USART0 shared with the Bootloader

//  GD32    QFP48   function
//  Vbat     1
//  PC13     2      LED
//  PC14     3      32KHz Osc
//  PC15     4      32KHz Osc
//  PA0     10      WAKE
//  PA1     11      Joy1 (x)
//  PA2     12      Joy2 (y)
//  PA3     13
//  PA4     14
//  PA5     15
//  PA6     16
//  PA7     17
//  PB0     18
//  PB1     19
//  PB10    21
//  PB11    22
//
//  PB12    25
//  PB13    26
//  PB14    27
//  PB15    28
//  PA8     29
//  PA9     30
//  PA10    31
//  PA11    32
//  PA12    33
//  PA15    38
//  PB3     39
//  PB4     40
//  PB5     41
//  PB6     42  USART0 TX (alt function)
//  PB7     43  USART0 RX (alt function)
//  PB8     45
//  PB9     46
//
// PA13 & PA14 are used for debugger

//
// pin/port defines
//

#define     LED_PIN           GPIO_PIN_13
#define     LED_PORT          GPIOC

#define     RS232_TXD_PIN     GPIO_PIN_9  // Also used by the Bootloader
#define     RS232_RXD_PIN     GPIO_PIN_10 // Also used by the Bootloader

#define     BOOT1             GPIO_PIN_2
#define     SWDIO             GPIO_PIN_13
#define     SWDCLK            GPIO_PIN_14

#define     RED_LED           GPIOC,LED_PIN
#define     RS232_TXD         GPIOA,RS232_TXD_PIN
#define     RS232_RXD         GPIOA,RS232_RXD_PIN

#define     USB_D_PLUS_PIN    GPIO_PIN_12
#define     USB_D_MINUS_PIN   GPIO_PIN_11

#define     USB_D_PLUS        GPIOA,USB_D_PLUS_PIN
#define     USB_D_MINUS       GPIOA,USB_D_MINUS_PIN

#define     JOYSTICK_X_PIN    GPIO_PIN_1  // PA1
#define     JOYSTICK_Y_PIN    GPIO_PIN_2  // PA2
#define     JOYSTICK_PORT     GPIOA

#define     BUTTON_PIN        GPIO_PIN_12  // PB12
#define     BUTTON_PORT       GPIOB 
#endif
