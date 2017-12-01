#ifndef __GAMSONG_ADC_H__
#define __GAMSONG_ADC_H__


extern uint16_t illum;

void ADC1_GPIO_init(void);
void ADC1_SENSOR_init(void);
void ADC1_2_IRQHandler(void);
void ADC1_InterruptConfigure(void);
void ADC1_RCC_Init(void);

#endif
