#ifndef __GAMSONG_ADC_H__
#define __GAMSONG_ADC_H__


//extern uint16_t illum;
extern uint32_t ADC_DualConvertedValueTab[3];


void ADC1_RCC_Init(void);
void ADC1_GPIO_init(void);
void ADC1_SENSOR_init(void);
void ADC1_EXEC(void);

#endif
