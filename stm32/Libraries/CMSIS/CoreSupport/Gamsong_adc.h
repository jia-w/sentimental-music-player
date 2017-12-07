#ifndef __GAMSONG_ADC_H__
#define __GAMSONG_ADC_H__


extern uint16_t sensor_temper;
extern uint16_t sensor_humid;
extern uint16_t sensor_illum;
extern uint16_t sensor_rain;
extern uint32_t ADC_DualConvertedValueTab[4];


void ADC1_RCC_Init(void);
void ADC1_GPIO_init(void);
void ADC1_SENSOR_init(void);
void ADC1_EXEC(void);

#endif
