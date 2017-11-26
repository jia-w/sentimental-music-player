#include <stdio.h>
#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_usart.h>
#include <stm32f10x_exti.h>

#include <string.h>
#include "Gamsong_usart.h"
#include "Gamsong_led.h"
#include "Gamsong_usart.h"

const char*	ON 			= "on";
const char* OFF 		= "off";

char Usart2_Msg[100] 	= "";
int  Usart2_Msg_Size 	= 0;


/** caution : AFIO -> APB2 **/
void USART1_RCC_Init(){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA |
                            RCC_APB2Periph_USART1, ENABLE);
}   

void USART2_RCC_Init(){
    RCC_APB1PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA |
                            RCC_APB1Periph_USART2, ENABLE);
}

void USART3_RCC_Init(){
    RCC_APB1PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB |
                            RCC_APB1Periph_USART3, ENABLE);
}   

void USART1_GPIO_Init(){
    GPIO_InitTypeDef Putty_TX;
	GPIO_InitTypeDef Putty_RX;
 
	Putty_TX.GPIO_Mode  	= GPIO_Mode_AF_PP;
	Putty_TX.GPIO_Pin  	 	= GPIO_Pin_9;
	Putty_TX.GPIO_Speed 	= GPIO_Speed_50MHz;
 
	Putty_RX.GPIO_Mode  	= GPIO_Mode_IN_FLOATING;
	Putty_RX.GPIO_Pin   	= GPIO_Pin_10;
	Putty_RX.GPIO_Speed 	= GPIO_Speed_50MHz;

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
	BlueTooth_RX.GPIO_Speed = GPIO_Speed_50MHz;

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
	Wifi_RX.GPIO_Speed 		= GPIO_Speed_50MHz;

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
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
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
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
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
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
}


void USART1_InterruptConfigure(){
    EXTI_InitTypeDef Putty_IR;
	NVIC_InitTypeDef Putty_IRQ;
 
	Putty_IR.EXTI_Line     							= EXTI_Line10;             
	Putty_IR.EXTI_LineCmd  							= ENABLE;               
	Putty_IR.EXTI_Mode     							= EXTI_Mode_Interrupt;         
	Putty_IR.EXTI_Trigger  							= EXTI_Trigger_Rising_Falling;
	
	Putty_IRQ.NVIC_IRQChannel                   	= USART1_IRQn;
	Putty_IRQ.NVIC_IRQChannelPreemptionPriority 	= 1;
	Putty_IRQ.NVIC_IRQChannelSubPriority        	= 1;
	Putty_IRQ.NVIC_IRQChannelCmd                	= ENABLE;

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
	BlueTooth_IRQ.NVIC_IRQChannelSubPriority        = 1;
	BlueTooth_IRQ.NVIC_IRQChannelCmd                = ENABLE;

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
	
	Wifi_IRQ.NVIC_IRQChannel                      	= USART2_IRQn;
	Wifi_IRQ.NVIC_IRQChannelPreemptionPriority    	= 0;
	Wifi_IRQ.NVIC_IRQChannelSubPriority           	= 0;
	Wifi_IRQ.NVIC_IRQChannelCmd                  	= ENABLE;

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource11);
	EXTI_Init(&Wifi_IR);
	NVIC_Init(&Wifi_IRQ);
}


void USART1_IRQHandler(){
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
		uint8_t Putty_msg = USART_ReceiveData(USART1);
		USART_SendData(USART1, Putty_msg);
		while(!(USART1->SR & USART_SR_TXE)){}
//		USART_SendData(USART2, Putty_msg);
//		while(!(USART2->SR & USART_SR_TXE)){}
		USART_SendData(USART3, Putty_msg);
		while(!(USART3->SR & USART_SR_TXE)){}
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
}     
void USART2_IRQHandler(){
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
		uint8_t BlueTooth_msg = USART_ReceiveData(USART2);
		USART_SendData(USART1, BlueTooth_msg);
		while(!(USART1->SR & USART_SR_TXE)){}

		if(BlueTooth_msg != '\n')
			Usart2_Msg[Usart2_Msg_Size++] = BlueTooth_msg;
		else{
			Usart2_Msg[Usart2_Msg_Size-1] ='\0';
			if(!strcmp(ON ,Usart2_Msg))
				LED_TurnOnAll();
			if(!strcmp(OFF,Usart2_Msg))
				LED_TurnOffAll();

			Usart2_Msg_Size = 0;
		}

		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
	}
}   
void USART3_IRQHandler(){
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
		uint8_t Wifi_msg = USART_ReceiveData(USART3);
		USART_SendData(USART1, Wifi_msg);
		while(!(USART1->SR & USART_SR_TXE)){}

		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
	}
}      
     