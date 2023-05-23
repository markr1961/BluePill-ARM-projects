#ifndef     _SERIAL_USART_H
#define     _SERIAL_USART_H

//
//  INCLUDE FILES
//

//
//  EXPORTED MACRO CONSTANTS
//
#define     TX_BUFF_SIZE           (1024)
#define     RX_BUFF_SIZE           (255)
#define     DEBUG_USART                 USART0

#define     DEBUG_SERIAL_BAUD_RATE      115200

void     InitSerial(void);
void     SerialRead(uint8_t *pMsg, uint8_t u8Bytes);
void     SerialWrite(uint8_t *pMsg, uint8_t u8Size);
#endif
