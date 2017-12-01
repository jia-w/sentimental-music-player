#include <misc.h>
#include <stdio.h>
#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_usart.h>
#include <stm32f10x_exti.h>
#include <stm32f10x_adc.h>
#include "Gamsong_adc.h"
#include "Gamsong_led.h"

uint16_t illum = 0;

void ADC1_RCC_Init() {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOC |  RCC_APB2Periph_ADC1, ENABLE);
}

void ADC1_InterruptConfigure(){
	NVIC_InitTypeDef NVIC_ADC_init;
	EXTI_InitTypeDef EXTI_ADC_init;
 
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource1);
	EXTI_ADC_init.EXTI_Line = EXTI_Line1;				// RX
	EXTI_ADC_init.EXTI_LineCmd = ENABLE; 				// Enable Interrupt
	EXTI_ADC_init.EXTI_Mode = EXTI_Mode_Interrupt;		// Interrupt mode
	EXTI_ADC_init.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_Init(&EXTI_ADC_init);
 
	NVIC_ADC_init.NVIC_IRQChannel = ADC1_2_IRQn;
	NVIC_ADC_init.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_ADC_init.NVIC_IRQChannelSubPriority = 0;
	NVIC_ADC_init.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_ADC_init);
	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
	ADC_Cmd(ADC1, ENABLE);
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
}

void ADC1_GPIO_init(){
	GPIO_InitTypeDef GPIOC_ADC_init;
	GPIOC_ADC_init.GPIO_Mode = GPIO_Mode_AIN;
	GPIOC_ADC_init.GPIO_Pin = GPIO_Pin_1;
	GPIOC_ADC_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIOC_ADC_init);
}

void ADC1_SENSOR_init() {
	ADC_InitTypeDef ADC_SENSOR_init;
	
	ADC_DeInit(ADC1);
	ADC_SENSOR_init.ADC_Mode = ADC_Mode_Independent;
	ADC_SENSOR_init.ADC_ScanConvMode = ENABLE;
	ADC_SENSOR_init.ADC_ContinuousConvMode = ENABLE;
	ADC_SENSOR_init.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_SENSOR_init.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_SENSOR_init.ADC_NbrOfChannel = 1;

	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_239Cycles5);
	ADC_Init(ADC1, &ADC_SENSOR_init);
}


void ADC1_2_IRQHandler(){
	uint16_t input;
	double result;
	input = ADC_GetConversionValue(ADC1);
	result = (double)input;

	illum = result;

	if(illum<=500){
		LED_TurnOnAll();
	}
	else if(illum<=1200){
		LED_TurnOff(0,0,0,1);
		LED_TurnOn(1,1,1,0);
	}
	else if(illum<=1700){
		LED_TurnOff(0,0,1,1);
		LED_TurnOn(1,1,0,0);
	}
	else if(illum<=2300){
		LED_TurnOff(0,1,1,1);
		LED_TurnOn(1,0,0,0);
	}
	else{
		LED_TurnOnAll();
	}
	ADC_ClearITPendingBit(ADC1,ADC_IT_EOC);
}


