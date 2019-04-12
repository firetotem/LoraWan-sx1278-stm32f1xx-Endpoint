#ifndef INC_HW_ADC_H_
#define INC_HW_ADC_H_

void HW_Adc_Init( void );
void HW_Adc_DeInit( void );

uint16_t HW_GetTemperatureLevel( void );
uint8_t HW_GetBatteryLevel( void );

#endif /* INC_HW_ADC_H_ */
