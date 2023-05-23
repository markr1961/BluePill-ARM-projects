
//
// Include Files
//
#include  "Hardware.h"

//
// Global Variable Definitions
//
bool  bTransmitting;
bool  bTxBuffEmpty, bRxBuffEmpty;
int txCount, rxCount, txPointer;

uint8_t txBuffer[TX_BUFF_SIZE];
uint8_t rxBuffer[RX_BUFF_SIZE];

// debug vars

uint32_t  USART0_IRQHandler_Counter;
uint32_t  u32UartOreErrCnt;   // USART Overrun error count
uint32_t  u32UartFeErrCnt;    // USART Framing error error count
uint32_t  u32UartNeErrCnt;    // USART Noise error count
uint32_t  u32UartPeErrCnt;    // USART Parity error count

//
// Local Function Prototypes
//
static void InitUSART0(void);

void InitSerial(void)
{
  bTransmitting = false, bTxBuffEmpty = true, bRxBuffEmpty = true;
  txCount = 0, rxCount = 0, txPointer= 0;
  memset (txBuffer, 0, sizeof(txBuffer));
  memset (rxBuffer, 0, sizeof(rxBuffer));
  // Initialize the UART hardware
  InitUSART0();

  // Configure NVIC for USART 0 interrupts. 
  // NVIC_SetPriority(USART0_IRQn, 0);
  // NVIC_EnableIRQ(USART0_IRQn);

}


static void InitUSART0(void)
{
  // USART0 GPIO Configuration
  //   PA9   ------> USART0_TX
  //  PA10   ------> USART0_RX

  /* enable COM GPIO clock */
  rcu_periph_clock_enable(RCU_GPIOA);
  /* connect port to USARTx_Tx */
  gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_9);
  /* connect port to USARTx_Rx */
  gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_10);
  /* configure USART Tx as alternate function push-pull */
  gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_9);
  gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_9);
  /* configure USART Rx as alternate function push-pull */
  gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_10);
  gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_10);

  /* USART interrupt configuration */
  nvic_irq_enable(USART0_IRQn, 0, 0);

  /* enable USART clock */
  rcu_periph_clock_enable(RCU_USART0);

  /* USART configure */
  usart_deinit(USART0); // sets to 8N1
  usart_baudrate_set(USART0, 115200U);

  usart_enable(USART0);

  // Clean up flags
  usart_flag_clear(USART0, USART_FLAG_RBNE);      // Rx buffer not empty
  usart_flag_clear(USART0, USART_FLAG_ORERR);   // Overrun error
  usart_flag_clear(USART0, USART_FLAG_FERR);  // Framing error
  usart_flag_clear(USART0, USART_FLAG_PERR);      // Parity error (parity not enabled)
  usart_flag_clear(USART0, USART_FLAG_TC);        // Transmit complete
  usart_flag_clear(USART0, USART_FLAG_IDLE);      // Idle
  usart_flag_clear(USART0, USART_FLAG_LBD);       // LIN Break Detection is not used
  usart_flag_clear(USART0, USART_FLAG_CTS);       // Flow control

  // Set up UART errors to handle or ignore as interrupts
  // Note that all the error flags get checked when there is a rx int
  usart_interrupt_disable(USART0, USART_INT_PERR);    // Parity error (no parity)
  usart_interrupt_disable(USART0, USART_INT_IDLE);  // Idle
  usart_interrupt_disable(USART0, USART_INT_LBD);   // LIN Break Detection is not used
  usart_interrupt_disable(USART0, USART_INT_CTS);   // Flow control

  // Note that all the error flags get checked when there is a rx int
  usart_interrupt_flag_clear(USART0, USART_INT_FLAG_RBNE);    // Rx buffer not empty
  usart_interrupt_flag_clear(USART0, USART_INT_FLAG_RBNE_ORERR);   // Overrun error
  usart_interrupt_flag_clear(USART0, USART_INT_FLAG_PERR);    // Framing error
  usart_interrupt_flag_clear(USART0, USART_INT_FLAG_PERR);    // Parity error (parity not enabled)
  usart_interrupt_flag_clear(USART0, USART_INT_FLAG_TC);      // Transmit complete
  usart_interrupt_flag_clear(USART0, USART_INT_FLAG_IDLE);    // Idle
  usart_interrupt_flag_clear(USART0, USART_INT_FLAG_LBD);     // LIN Break Detection is not used
  usart_interrupt_flag_clear(USART0, USART_INT_FLAG_CTS);     // Flow control

  usart_interrupt_disable(USART0, USART_INT_TBE);
  usart_interrupt_disable(USART0, USART_INT_TC);

  // Flush the read buffer
  usart_command_enable(USART0, USART_CMD_RXFCMD);

  // Enable receiver but leave the transmitter off until needed
  usart_receive_config(USART0, USART_RECEIVE_ENABLE);
  usart_transmit_config(USART0, USART_TRANSMIT_DISABLE);

  // Enable receiver int but leave the transmitter off until needed
  usart_interrupt_enable(USART0, USART_INT_RBNE);
}

void USART0_IRQHandler(void)
{
  USART0_IRQHandler_Counter++;

  // gpio_bit_set(TEST_OUT);    // Debug out 1 - high

  // Check individual errors for debug error counts
  if(usart_flag_get(USART0, USART_FLAG_ORERR))
  { // Overrun Error
    // Clear flag and update error count
    usart_flag_clear(USART0, USART_FLAG_ORERR);
    u32UartOreErrCnt++;
  }

  if(usart_flag_get(USART0, USART_FLAG_FERR))
  { // Framing Error
    // Clear flag and update error count
    usart_flag_clear(USART0, USART_FLAG_FERR);
    u32UartFeErrCnt++;
  }

  if(usart_flag_get(USART0, USART_FLAG_NERR))
  { // Noise Error
    // Clear flag and update error count
    usart_flag_clear(USART0, USART_FLAG_NERR);
    u32UartNeErrCnt++;
  }

  if(usart_flag_get(USART0, USART_FLAG_PERR))
  { // Parity Error
    // Clear flag and update error count
    usart_flag_clear(USART0, USART_FLAG_PERR);
    u32UartPeErrCnt++;
  }

  /*
    TX Section
  */
  // Is tx int enabled and is this a tx empty int?
  if(usart_interrupt_flag_get(USART0, USART_INT_FLAG_TBE)
     && usart_flag_get(USART0, USART_FLAG_TBE))
  {
    // Clear the pending TX IRQ
    usart_interrupt_flag_clear(USART0, USART_INT_FLAG_TBE);
    
    if(bTransmitting)
    {
      if (txCount)
      { // Send the character
        usart_data_transmit(USART0, txBuffer[txPointer++]);
        txCount--;
      }
      else
      {
        // No, disable the interrupt. (SerialWrite() will enable it again).
        usart_interrupt_disable(USART0, USART_INT_TBE);
        bTransmitting = false;
        bTxBuffEmpty = true;
      }
    }
    else  // should never get here!
      DEBUG_BREAK();
  }

  //Check for received character
  if(usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE)
      && usart_flag_get(USART0, USART_FLAG_RBNE))
  {
    //Clear the interrupt
    usart_interrupt_flag_clear(USART0, USART_INT_FLAG_RBNE);

    //Read the received character
    rxBuffer[rxCount++] = (char) usart_data_receive(USART0);
    bRxBuffEmpty = false;
  }

  // gpio_bit_reset(TEST_OUT);    // Debug out 1 - low
}
