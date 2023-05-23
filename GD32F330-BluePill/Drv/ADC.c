//
// Include Files
//
#include "Hardware.h"

//
// Macro Constants
//
#define     JOYSTICK_X_CHANNEL ADC_CHANNEL_1
#define     JOYSTICK_Y_CHANNEL ADC_CHANNEL_2
#define     TEMP_CHANNEL       ADC_CHANNEL_16
#define     V_REF_CHANNEL      ADC_CHANNEL_17
#define     V_BATT_CHANNEL     ADC_CHANNEL_18

#define     ADC_BITS           12
#define     MAX_ADC_RESULT     ((1 << ADC_BITS) - 1)

#define Vtemp_mV   1430      // Vtemp @ 25C in mVolt
#define dVdT_mV    4.3       // Vtemp slope in mVolt
#define Vcc_mV     3300      // Vcc in mVolt
#define Tref       25        // reference temperature in C.
//  Temp = 25C + dT
//  dT = dV / (4.3mV/C)
//  dV = [ADC_value/ADC_MAX * Vcc] - Vtemp
//  ignoring precision and consequences of integer math, the equation would be:
//  s32Temp = 25 + (((ADC value/MAX_ADC_RESULT) * Vcc_mV ) - V25C_mV)/dVdt_mV ;
//  to preserve precision, ADC value is cast to 32bit and shifted up 4 bits (x16)
//  1024 is used for mV to uV to make the math "shifter friendly"

#define Vref_mV   1200  // Vref @ 25C in mVolt

//
volatile uint16_t u16ADCResult[NUM_ADC_CHAN] = {0};  //  raw ADC results

///
// @brief init the common components for the ADC
void InitADC(void)
{
  // Configure GPIO used by ADC pins
  // Peripheral clock enable
  rcu_periph_clock_enable(RCU_GPIOA);

  //  ADC1 GPIO Configuration
  //  PA0-WKUP  --> ADC1_IN0
  //  PA1   ------> ADC1_IN1  --> joystick X
  //  PA2   ------> ADC1_IN2  --> joystick Y
  //  PA3   ------> ADC1_IN3

  gpio_mode_set(JOYSTICK_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, JOYSTICK_X_PIN|JOYSTICK_Y_PIN);

  // Enable ADC clock.
  rcu_periph_clock_enable(RCU_ADC);

  rcu_adc_clock_config(RCU_ADCCK_APB2_DIV6);  //= 1MHz with APB2 = 6 MHz,
  GetClockFreq();

  // Reset ADC to default state
  adc_deinit();

  adc_resolution_config(ADC_RESOLUTION_12B);  // default
  adc_data_alignment_config(ADC_DATAALIGN_RIGHT); // default

//  // this produces continuous ADC conversions and DMA IRQs:
//  adc_special_function_config(ADC_CONTINUOUS_MODE, ENABLE);
//  adc_special_function_config(ADC_SCAN_MODE, ENABLE);
//  adc_external_trigger_source_config(ADC_REGULAR_CHANNEL, ADC_EXTTRIG_REGULAR_NONE);
//  adc_external_trigger_config(ADC_REGULAR_CHANNEL, ENABLE);
//  // after setting up channels, enable the ADC and DMA then start it running by:
//  adc_software_trigger_enable(ADC_REGULAR_CHANNEL);

  // this runs a single scan with DMA IRQ:
  adc_external_trigger_source_config(ADC_REGULAR_CHANNEL, ADC_EXTTRIG_REGULAR_NONE);
  adc_external_trigger_config(ADC_REGULAR_CHANNEL, ENABLE);
  adc_discontinuous_mode_config(ADC_REGULAR_CHANNEL, 1U); // why 3?
 
  adc_channel_length_config(ADC_REGULAR_CHANNEL, 5);  // A1,A2,Vref,temp,Vbat
  // Sample time is set where the channel is selected.
  adc_regular_channel_config(0U, ADC_CHANNEL_1, ADC_SAMPLETIME_55POINT5);
  adc_regular_channel_config(1U, ADC_CHANNEL_2, ADC_SAMPLETIME_55POINT5);
  adc_regular_channel_config(2U, ADC_CHANNEL_16, ADC_SAMPLETIME_55POINT5);  //temp, 17.1uS
  adc_regular_channel_config(3U, ADC_CHANNEL_17, ADC_SAMPLETIME_55POINT5);  //Vref, 17.1uS
  adc_regular_channel_config(4U, ADC_CHANNEL_18, ADC_SAMPLETIME_55POINT5);  //Vbat, ??? uS
  
  /* enable ADC interface */
  adc_tempsensor_vrefint_enable();
  adc_vbat_enable();

//   // Configure NVIC for ADC complete interrupt.
//   NVIC_SetPriority(ADC_CMP_IRQn, 3);
//   NVIC_EnableIRQ(ADC_CMP_IRQn);

adc_enable();

  delay_ms(1);
  
  /* ADC calibration and reset calibration */
  adc_calibration_enable();

  delay_ms(1);

  /* ADC DMA function enable */
  adc_dma_mode_enable();
  
  /* make sure nothing is pending */
  adc_interrupt_flag_clear(ADC_INT_EOC);
  /* enable the end of conversion IRQ */
  //adc_interrupt_enable(ADC_INT_EOC);
  
  /* Start the first conversion */
  adc_software_trigger_enable(ADC_REGULAR_CHANNEL);
}

void GenerateADC(void)
{
  adc_software_trigger_enable(ADC_REGULAR_CHANNEL);
}

 void ReadADC(void)
 {
#ifdef DEBUG
        printf("//*******************************//\r\n");
        printf("ADC joystick X channel data = %04X\r\n", u16ADCResult[JOYSTICK_X_INDEX]);
        printf("ADC joystick Y channel data = %04X\r\n", u16ADCResult[JOYSTICK_Y_INDEX]);
        printf("ADC temp channel data = %04X\r\n", u16ADCResult[TEMPERATURE_INDEX]);
        printf("ADC Vref channel data = %04X\r\n", u16ADCResult[REFERENCE_INDEX]);
        printf("ADC VBat channel data = %04X\r\n", u16ADCResult[BATTERY_INDEX]);
        printf("\r\n");
#endif
}

int readJoystick( adc_index joystick)
{
  int percent = -1;
  switch (joystick)
  {
  case JOYSTICK_X_INDEX:
  case JOYSTICK_Y_INDEX:
    percent = (u16ADCResult[joystick]*100)/MAX_ADC_RESULT;
    break;
  default:
    DEBUG_BREAK();
  }
  return percent;
}

int8_t s8GetTempRead(void)
{
  int32_t s32Temp, s32Volt;
  int8_t  s8result = 0;

  // Get the ADC reading
  s32Volt = (u16ADCResult[TEMPERATURE_INDEX])<<4; // shift up to preserve precision for following ops.

  if (s32Volt != 0)
  {
    s32Volt = s32Volt * (int32_t)((Vcc_mV*1024)/(MAX_ADC_RESULT)); // normalize ADC value for Vcc & ADC size
    s32Volt = s32Volt - (int32_t)(Vtemp_mV*1024<<4);     // subtract the Voltage offset at 25C
    s32Temp = s32Volt / ((int)(dVdT_mV*16));     // voltage/slope = temperature (also remove x16 multiplier)
    s32Temp = 25*1024 - s32Temp;                // subtract off the standard Temp
    s8result = (int8_t) (s32Temp/1024);         // remove the 1024 multiplier
  }
  return (s8result);
}

uint16_t u16GetVccRead(void)
{
  uint32_t u32temp=0;

  // Get the ADC reading
  u32temp = (uint32_t) u16ADCResult[REFERENCE_INDEX];

  if (u32temp)  // make sure the ADC didn't hark and return 0
    u32temp = (Vref_mV * MAX_ADC_RESULT) / u32temp;

  return ((uint16_t)u32temp);
}
uint16_t u16GetVbatRead(void)
{
  uint32_t u32temp=0;

  // Get the ADC reading
  u32temp = (uint32_t) u16ADCResult[BATTERY_INDEX];

  if (u32temp)  // make sure the ADC didn't hark and return 0
    u32temp = (Vcc_mV * 2 * u32temp)/MAX_ADC_RESULT;

  return ((uint16_t)u32temp);
}
