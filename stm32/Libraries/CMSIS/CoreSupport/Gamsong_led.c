#include <misc.h>
#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>

#include <stdint.h>
#include "Gamsong_led.h"


void LED_RCC_Init(){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
}

void LED_Configure(){
    GPIO_InitTypeDef LED;

	LED.GPIO_Mode   = GPIO_Mode_Out_PP;
	LED.GPIO_Speed  = GPIO_Speed_50MHz;
	LED.GPIO_Pin    = (GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7);

	GPIO_Init(GPIOD, &LED);
}

void LED_TurnOnAll(){
    GPIO_SetBits  (GPIOD,GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7);
}
	
void LED_TurnOffAll(){
    GPIO_ResetBits(GPIOD,GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7);
}

void LED_TurnOn(uint16_t LED1, uint16_t LED2, uint16_t LED3, uint16_t LED4){
    uint16_t    GPIO_Pin = 0x0000;

    if(LED1)    GPIO_Pin |= GPIO_Pin_2;
    if(LED2)    GPIO_Pin |= GPIO_Pin_3;
    if(LED3)    GPIO_Pin |= GPIO_Pin_4;
    if(LED4)    GPIO_Pin |= GPIO_Pin_7;

    GPIO_SetBits  (GPIOD,GPIO_Pin);
}

void LED_TurnOff(uint16_t LED1, uint16_t LED2, uint16_t LED3, uint16_t LED4){
    uint16_t    GPIO_Pin = 0x0000;

    if(LED1)    GPIO_Pin |= GPIO_Pin_2;
    if(LED2)    GPIO_Pin |= GPIO_Pin_3;
    if(LED3)    GPIO_Pin |= GPIO_Pin_4;
    if(LED4)    GPIO_Pin |= GPIO_Pin_7;

    GPIO_ResetBits(GPIOD,GPIO_Pin);
}
