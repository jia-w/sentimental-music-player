#include <misc.h>
#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_usart.h>
#include <stm32f10x_exti.h>

#include <string.h>
#include <stdint.h>
#include "Gamsong_usart.h"
#include "Gamsong_led.h"
#include "Gamsong_usart.h"

uint16_t USART1_READY = 0;
uint16_t USART2_READY = 0;
uint16_t USART3_READY = 0;

uint8_t Putty_msg, BlueTooth_msg, Wifi_msg;
uint8_t USART1_M[1000] 	= "";
uint8_t USART2_M[1000] 	= "";
uint8_t USART3_M[1000]   = "";

const char*	ON 			= "on";
const char* OFF 		= "off";
const char* AT			= "AT+RST";


int USART1_INDEX = 0, USART1_SEND = 0;
int USART2_INDEX = 0, USART2_SEND = 0;
int USART3_INDEX = 0, USART3_SEND = 0;


/** caution : AFIO -> APB2 **/
void USART1_RCC_Init(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);
}

void USART2_RCC_Init(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);
}

void USART3_RCC_Init(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);
}

void USART1_GPIO_Init(){
	GPIO_InitTypeDef Putty_TX;
	GPIO_InitTypeDef Putty_RX;

	Putty_TX.GPIO_Mode  	= GPIO_Mode_AF_PP;
	Putty_TX.GPIO_Pin  	 	= GPIO_Pin_9;
	Putty_TX.GPIO_Speed 	= GPIO_Speed_50MHz;

	Putty_RX.GPIO_Mode  	= GPIO_Mode_IN_FLOATING;
	Putty_RX.GPIO_Pin   	= GPIO_Pin_10;

	GPIO_Init(GPIOA, &Putty_TX);
	GPIO_Init(GPIOA, &Putty_RX);
}

void USART2_GPIO_Init(){
	GPIO_InitTypeDef BlueTooth_TX;
	GPIO_InitTypeDef BlueTooth_RX;

	BlueTooth_TX.GPIO_Mode  = GPIO_Mode_AF_PP;
	BlueTooth_TX.GPIO_Pin   = GPIO_Pin_2;
	BlueTooth_TX.GPIO_Speed = GPIO_Speed_50MHz;

	BlueTooth_RX.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	BlueTooth_RX.GPIO_Pin   = GPIO_Pin_3;

	GPIO_Init(GPIOA, &BlueTooth_TX);
	GPIO_Init(GPIOA, &BlueTooth_RX);
}

void USART3_GPIO_Init(){
	GPIO_InitTypeDef Wifi_TX;
	GPIO_InitTypeDef Wifi_RX;

	Wifi_TX.GPIO_Mode  		= GPIO_Mode_AF_PP;
	Wifi_TX.GPIO_Pin   		= GPIO_Pin_10;
	Wifi_TX.GPIO_Speed 		= GPIO_Speed_50MHz;

	Wifi_RX.GPIO_Mode  		= GPIO_Mode_IN_FLOATING;
	Wifi_RX.GPIO_Pin   		= GPIO_Pin_11;

	GPIO_Init(GPIOB, &Wifi_TX);
	GPIO_Init(GPIOB, &Wifi_RX);
}

void USART1_Configure(int baudrate){
	USART_InitTypeDef Putty;

	Putty.USART_BaudRate 				= baudrate;
	Putty.USART_WordLength 				= USART_WordLength_8b;
	Putty.USART_StopBits 				= USART_StopBits_1;
	Putty.USART_Parity 					= USART_Parity_No;
	Putty.USART_HardwareFlowControl 	= USART_HardwareFlowControl_None;
	Putty.USART_Mode 					= USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &Putty);
	USART_Cmd(USART1, ENABLE);
}

void USART2_Configure(int baudrate){
	USART_InitTypeDef BlueTooth;

	BlueTooth.USART_BaudRate			= baudrate;
	BlueTooth.USART_WordLength 			= USART_WordLength_8b;
	BlueTooth.USART_StopBits 			= USART_StopBits_1;
	BlueTooth.USART_Parity 				= USART_Parity_No;
	BlueTooth.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	BlueTooth.USART_Mode 				= USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART2, &BlueTooth);
	USART_Cmd(USART2, ENABLE);

}

void USART3_Configure(int baudrate){
	USART_InitTypeDef Wifi;

	Wifi.USART_BaudRate 				= baudrate;
	Wifi.USART_WordLength 				= USART_WordLength_8b;
	Wifi.USART_StopBits 				= USART_StopBits_1;
	Wifi.USART_Parity					= USART_Parity_No;
	Wifi.USART_HardwareFlowControl 		= USART_HardwareFlowControl_None;
	Wifi.USART_Mode 					= USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART3, &Wifi);
	USART_Cmd(USART3, ENABLE);
}




void USART1_InterruptConfigure(){
	EXTI_InitTypeDef Putty_IR;
	NVIC_InitTypeDef Putty_IRQ;

	Putty_IR.EXTI_Line     							= EXTI_Line10;
	Putty_IR.EXTI_LineCmd  							= ENABLE;
	Putty_IR.EXTI_Mode     							= EXTI_Mode_Interrupt;
	Putty_IR.EXTI_Trigger  							= EXTI_Trigger_Rising_Falling;

	Putty_IRQ.NVIC_IRQChannel                   	= USART1_IRQn;
	Putty_IRQ.NVIC_IRQChannelPreemptionPriority 	= 0;
	Putty_IRQ.NVIC_IRQChannelSubPriority        	= 1;
	Putty_IRQ.NVIC_IRQChannelCmd                	= ENABLE;

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource10);

	EXTI_Init(&Putty_IR);
	NVIC_Init(&Putty_IRQ);
}
void USART2_InterruptConfigure(){
	EXTI_InitTypeDef BlueTooth_IR;
	NVIC_InitTypeDef BlueTooth_IRQ;

	BlueTooth_IR.EXTI_Line        					= EXTI_Line3;
	BlueTooth_IR.EXTI_LineCmd     					= ENABLE;
	BlueTooth_IR.EXTI_Mode        					= EXTI_Mode_Interrupt;
	BlueTooth_IR.EXTI_Trigger     					= EXTI_Trigger_Rising_Falling;

	BlueTooth_IRQ.NVIC_IRQChannel                   = USART2_IRQn;
	BlueTooth_IRQ.NVIC_IRQChannelPreemptionPriority = 0;
	BlueTooth_IRQ.NVIC_IRQChannelSubPriority        = 0;
	BlueTooth_IRQ.NVIC_IRQChannelCmd                = ENABLE;

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource3);
	EXTI_Init(&BlueTooth_IR);
	NVIC_Init(&BlueTooth_IRQ);

}
void USART3_InterruptConfigure(){
	EXTI_InitTypeDef Wifi_IR;
	NVIC_InitTypeDef Wifi_IRQ;

	Wifi_IR.EXTI_Line     							= EXTI_Line11;
	Wifi_IR.EXTI_LineCmd  							= ENABLE;
	Wifi_IR.EXTI_Mode     							= EXTI_Mode_Interrupt;
	Wifi_IR.EXTI_Trigger  							= EXTI_Trigger_Rising_Falling;

	Wifi_IRQ.NVIC_IRQChannel                      	= USART3_IRQn;
	Wifi_IRQ.NVIC_IRQChannelPreemptionPriority    	= 0;
	Wifi_IRQ.NVIC_IRQChannelSubPriority           	= 0;
	Wifi_IRQ.NVIC_IRQChannelCmd                  	= ENABLE;

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource11);
	EXTI_Init(&Wifi_IR);
	NVIC_Init(&Wifi_IRQ);
}

void USART_SendPeriod(USART_TypeDef *USARTx){
	USART_SendData(USARTx, (uint8_t)'\r');
	while(!(USARTx->SR & USART_SR_TXE)){}

	USART_SendData(USARTx, (uint8_t)'\n');
	while(!(USARTx->SR & USART_SR_TXE)){}

}

void USART_SendString(USART_TypeDef *USARTx, uint8_t *buffer, int buffer_SZ) {
	int i;
	for (i = 0 ; i < buffer_SZ; i++) {
		USART_SendData(USARTx, buffer[i]);
		while(!(USARTx->SR & USART_SR_TXE)){}
	}
	USART_SendPeriod(USARTx);

}

void USART1_Handler_Method(){
	LED_TurnOn(1,0,1,0);
	USART_SendString(USART1,USART1_M, USART1_SEND);
	USART_SendString(USART2,USART1_M, USART1_SEND);
	USART_SendString(USART3,USART1_M, USART1_SEND);
	LED_TurnOff(1,0,0,0);
}

void USART2_Handler_Method(){
	LED_TurnOnAll();
	USART_SendString(USART1,USART2_M, USART2_SEND);
	LED_TurnOffAll();
}

void USART3_Handler_Method(){
	LED_TurnOnAll();
	USART_SendString(USART1,USART3_M, USART3_SEND);
	LED_TurnOffAll();
}

void USART1_IRQHandler(){
	LED_TurnOn(1,1,1,0);
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
		USART1_M[USART1_INDEX++] = USART_ReceiveData(USART1);

		//if(USART1_M[USART1_INDEX-1] == '\r' || USART1_M[USART1_INDEX-1] == '\n'){
		if(USART1_M[USART1_INDEX-1] == '\r'){
			USART1_SEND 	= USART1_INDEX-1;
			USART1_INDEX 	= 0;
			USART1_READY 	= 1;
		}
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
}
void USART2_IRQHandler(){
	LED_TurnOn(1,0,1,1);
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
		USART2_M[USART2_INDEX++] = USART_ReceiveData(USART2);

		//if(USART2_M[USART2_INDEX-1] == '\r' || USART2_M[USART2_INDEX-1] == '\n'){
		if(USART2_M[USART2_INDEX-1] == '\n'){
			USART2_SEND 	= USART2_INDEX-1;
			USART2_INDEX 	= 0;
			USART2_READY 	= 1;
		}
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
	}
}
void USART3_IRQHandler(){
	LED_TurnOn(1,0,1,1);
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
		USART3_M[USART3_INDEX++] = USART_ReceiveData(USART3);

		if(USART3_M[USART3_INDEX-1] == '\n') {
			USART3_SEND 	= USART3_INDEX-1;
			USART3_INDEX 	= 0;
			USART3_READY 	= 1;
		}
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
	}
}

