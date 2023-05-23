///
// @brief various system IRQ handlers.
//

#include "Hardware.h"

volatile bool button_pressed;

// Vector Number 21,EXTI Line 0 and EXTI Line 1
void EXTI0_1_IRQHandler(void)
{
  DEBUG_BREAK();
}

// Vector Number 22,EXTI Line 2 and EXTI Line 3
void EXTI2_3_IRQHandler(void)
{
  DEBUG_BREAK();
}

// Vector Number 23,EXTI Line 4 to EXTI Line 15
void EXTI4_15_IRQHandler(void)
{
  if(RESET != exti_interrupt_flag_get(EXTI_12)) 
  {
    button_pressed = true;
    exti_interrupt_flag_clear(EXTI_12);
  }
  else
    DEBUG_BREAK();
}

uint32_t adcCount, elseCount;
// Vector Number 28,ADC and Comparator 1-2 
void ADC_CMP_IRQHandler(void)
{
  if (RESET != adc_interrupt_flag_get(ADC_INT_FLAG_EOC))
  {
    adc_interrupt_flag_clear(ADC_INT_FLAG_EOC);
    adcCount++;
  }
#ifdef DEBUG
  else
    elseCount++;
  //    DEBUG_BREAK();
#endif
}

uint32_t dmaCount, lastDMAtick;
// VectorNumber 25,DMA Channel 0
void DMA_Channel0_IRQHandler(void)
{
  if (dma_interrupt_flag_get(DMA_CH0, DMA_INT_FLAG_FTF))
  {
    dma_interrupt_flag_clear(DMA_CH0, DMA_INT_FLAG_FTF);
    dmaCount++;
    lastDMAtick = SyctickCounter;
  }
  else
    DEBUG_BREAK();
  }

// VectorNumber 26,DMA Channel 1 and DMA Channel 2
void DMA_Channel1_2_IRQHandler(void)
{
  DEBUG_BREAK();
}

// VectorNumber 27,DMA Channel 3 and DMA Channel 4
void DMA_Channel3_4_IRQHandler(void)
{
  DEBUG_BREAK();
}

// VectorNumber 41,SPI0
void SPI0_IRQHandler(void)
{
  DEBUG_BREAK();
}

// VectorNumber 42,SPI1
void SPI1_IRQHandler(void)
{
  DEBUG_BREAK();
}

// VectorNumber 38,TIMER16
void TIMER16_IRQHandler(void)
{
  DEBUG_BREAK();
}

// VectorNumber 39,I2C0 Event 
void I2C0_EV_IRQHandler(void)
{
  DEBUG_BREAK();
}

// VectorNumber 40,I2C1 Event
void I2C1_EV_IRQHandler(void)
{
  DEBUG_BREAK();
}

// VectorNumber 48,I2C0 Error
void I2C0_ER_IRQHandler(void)
{
  DEBUG_BREAK();
}

// VectorNumber 50,I2C1 Error
void I2C1_ER_IRQHandler(void)
{
  DEBUG_BREAK();
}
