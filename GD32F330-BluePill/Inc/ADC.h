
#ifndef _ADC_H
#define _ADC_H

typedef enum
{
  JOYSTICK_X_INDEX,   // CH1
  JOYSTICK_Y_INDEX,   // CH2
  TEMPERATURE_INDEX,  // CH16
  REFERENCE_INDEX,    // CH17
  BATTERY_INDEX,      // CH18
  NUM_ADC_CHAN,
} adc_index;

void InitADC(void);
void GenerateADC(void);
void ReadADC(void);
int readJoystick(adc_index);
int8_t s8GetTempRead(void);
uint16_t u16GetVccRead(void);
uint16_t u16GetVbatRead(void);

#endif