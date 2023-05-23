///
// @brief DMA control for ADC
//

#include "Hardware.h"

extern volatile uint16_t u16ADCResult[];

void InitDMA(void)
{
    /* enable DMA clock */
    rcu_periph_clock_enable(RCU_DMA);

    /* ADC_DMA_channel configuration */
    dma_parameter_struct dma_data_parameter;

   // Configure NVIC for DMA Ch0 interrupts.
   NVIC_SetPriority(DMA_Channel0_IRQn, 2);
   NVIC_EnableIRQ(DMA_Channel0_IRQn);

    /* ADC DMA_channel configuration */
    dma_deinit(DMA_CH0);

    /* initialize DMA single data mode */
    dma_data_parameter.periph_addr  = (uint32_t)(&ADC_RDATA);
    dma_data_parameter.periph_inc   = DMA_PERIPH_INCREASE_DISABLE;
    dma_data_parameter.memory_addr  = (uint32_t)(u16ADCResult);
    dma_data_parameter.memory_inc   = DMA_MEMORY_INCREASE_ENABLE;
    dma_data_parameter.periph_width = DMA_PERIPHERAL_WIDTH_16BIT;
    dma_data_parameter.memory_width = DMA_MEMORY_WIDTH_16BIT;
    dma_data_parameter.direction    = DMA_PERIPHERAL_TO_MEMORY;
    dma_data_parameter.number       = 5U;
    dma_data_parameter.priority     = DMA_PRIORITY_MEDIUM;
    dma_init(DMA_CH0, &dma_data_parameter);

    dma_circulation_enable(DMA_CH0);
    
    dma_interrupt_enable(DMA_CH0, DMA_INT_FTF);

    /* enable DMA channel */
    dma_channel_enable(DMA_CH0);

}
