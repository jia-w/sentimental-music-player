#include <misc.h>
#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>

#include "Gamsong_led.h"



void LED_RCC_Init(){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
}

void LED_Configure(){
    GPIO_InitTypeDef LED;

	LED.GPIO_Mode = GPIO_Mode_Out_PP;
	LED.GPIO_Speed = GPIO_Speed_50MHz;

	LED.GPIO_Pin = (GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7);

	GPIO_Init(GPIOD, &LED);
}

void LED_TurnAllOn(){
    GPIO_SetBits(GPIOD,GPIO_Pin_2);
	GPIO_SetBits(GPIOD,GPIO_Pin_3);
	GPIO_SetBits(GPIOD,GPIO_Pin_4);	
	GPIO_SetBits(GPIOD,GPIO_Pin_7);
}
	
void LED_TurnAllOff(){
    GPIO_ResetBits(GPIOD, GPIO_Pin_2);
    GPIO_ResetBits(GPIOD, GPIO_Pin_3);
    GPIO_ResetBits(GPIOD, GPIO_Pin_4);
    GPIO_ResetBits(GPIOD, GPIO_Pin_7);
}

void LED_TurnOn(bool LED1, bool LED2, bool LED3, bool LED4){
    if(LED1)    GPIO_SetBits(GPIOD,GPIO_Pin_2);
    if(LED2)    GPIO_SetBits(GPIOD,GPIO_Pin_3);
    if(LED3)    GPIO_SetBits(GPIOD,GPIO_Pin_4);
    if(LED4)    GPIO_SetBits(GPIOD,GPIO_Pin_7);
}
void LED_TurnOff(bool LED1, bool LED2, bool LED3, bool LED4){
    if(LED1)    GPIO_ResetBits(GPIOD,GPIO_Pin_2);
    if(LED2)    GPIO_ResetBits(GPIOD,GPIO_Pin_3);
    if(LED3)    GPIO_ResetBits(GPIOD,GPIO_Pin_4);
    if(LED4)    GPIO_ResetBits(GPIOD,GPIO_Pin_7);
}