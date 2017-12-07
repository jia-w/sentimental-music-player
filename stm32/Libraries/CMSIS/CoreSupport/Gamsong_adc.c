#include <misc.h>
#include <core_cm3.h>
#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_usart.h>
#include <stm32f10x_exti.h>
#include <stm32f10x_adc.h>
#include <stm32f10x_dma.h>
#include "Gamsong_adc.h"
#include "Gamsong_led.h"
#include <math.h>
#include <lcd.h>

uint16_t illum = 0;
uint32_t ADC_DualConvertedValueTab[3] = {0,0,0};



void ADC1_RCC_Init() {
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_ADC1, ENABLE);
}


void ADC1_GPIO_init(){
	GPIO_InitTypeDef GPIOC_ADC_init;
	GPIOC_ADC_init.GPIO_Mode = GPIO_Mode_AIN; //set as analog input
  	GPIOC_ADC_init.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3; //set to pin 1, 2,3

	GPIO_Init(GPIOC, &GPIOC_ADC_init);
}

void ADC1_SENSOR_init() {

	ADC_InitTypeDef ADC_SENSOR_init;
	DMA_InitTypeDef DMA_InitStructure;

	RCC_ADCCLKConfig(RCC_PCLK2_Div6); //clock for ADC (max 14MHz, 72/6=12MHz)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //enable ADC clock

	DMA_DeInit(DMA1_Channel1);
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_DualConvertedValueTab;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 3;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word; // 32bit
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word; // 32bit
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	DMA_Cmd(DMA1_Channel1, ENABLE);
		
	ADC_DeInit(ADC1);
	//configure ADC1 parameters
	ADC_SENSOR_init.ADC_Mode = ADC_Mode_Independent;
	ADC_SENSOR_init.ADC_ScanConvMode = ENABLE;
	ADC_SENSOR_init.ADC_ContinuousConvMode = ENABLE;
	ADC_SENSOR_init.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_SENSOR_init.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_SENSOR_init.ADC_NbrOfChannel  = 3;
	ADC_Init(ADC1, &ADC_SENSOR_init);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_239Cycles5); //PC1 as Input
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 2, ADC_SampleTime_239Cycles5); //PC2 as Input
	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 3, ADC_SampleTime_239Cycles5); //PC2 as Input
	ADC_DMACmd(ADC1, ENABLE);
	//enable
	ADC_Cmd(ADC1, ENABLE);

	//Calibrate ADC *optional?
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void ADC1_EXEC() {
	double m1 = -6.6875e1, m2 = 5.34057e-2;
	double m3 = -12.5e0, m4= 3.05175e-2;

	uint16_t temper = m1 + ((uint16_t)ADC_DualConvertedValueTab[0]*m2);
	uint16_t humid = m3 + ((uint16_t)ADC_DualConvertedValueTab[1]*m4);
	uint16_t illum = (uint16_t)ADC_DualConvertedValueTab[2];


	LCD_ShowString(1,  1, "temper", BLACK, WHITE);
	LCD_ShowString(1,  50, "humid", BLACK, WHITE);
	LCD_ShowString(1,  100, "illum", BLACK, WHITE);
	LCD_ShowString(1,  150, "Mon_team04", BLACK, WHITE);
	LCD_ShowNum(200,  1, temper, 4, BLACK, WHITE);
	LCD_ShowNum(200, 50,  humid, 4, BLACK, WHITE);
	LCD_ShowNum(200, 100,  illum, 4, BLACK, WHITE);
}
