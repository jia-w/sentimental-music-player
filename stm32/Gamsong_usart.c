#include <misc.h>
#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_usart.h>
#include <stm32f10x_exti.h>

#include <stdio.h>
#include "Gamsong_usart.h"



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
    GPIO_InitTypeDef Putty_TX;      //Tx
	GPIO_InitTypeDef Putty_RX;      //Rx -> interrupt
 
	Putty_TX.GPIO_Mode  = GPIO_Mode_AF_PP;
	Putty_TX.GPIO_Pin   = GPIO_Pin_9;
	Putty_TX.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &Putty_TX);
 
	Putty_RX.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	Putty_RX.GPIO_Pin   = GPIO_Pin_10;
	Putty_RX.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &Putty_RX);
}   

void USART2_GPIO_Init(){
    GPIO_InitTypeDef BlueTooth_TX;      //Tx
	GPIO_InitTypeDef BlueTooth_RX;      //Rx -> interrupt
 
	BlueTooth_TX.GPIO_Mode  = GPIO_Mode_AF_PP;
	BlueTooth_TX.GPIO_Pin   = GPIO_Pin_2;
	BlueTooth_TX.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &BlueTooth_TX);
 
	BlueTooth_RX.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	BlueTooth_RX.GPIO_Pin   = GPIO_Pin_3;
	BlueTooth_RX.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &BlueTooth_RX);
}  

void USART3_GPIO_Init(){
    GPIO_InitTypeDef WIFI_TX;      //Tx
	GPIO_InitTypeDef WIFI_RX;      //Rx -> interrupt
 
	WIFI_TX.GPIO_Mode  = GPIO_Mode_AF_PP;
	WIFI_TX.GPIO_Pin   = GPIO_Pin_10;
	WIFI_TX.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &WIFI_TX);
 
	WIFI_RX.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	WIFI_RX.GPIO_Pin   = GPIO_Pin_11;
	WIFI_RX.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &WIFI_RX);
}

void USART1_Configure(int baudrate){
    USART_InitTypeDef Putty;
 
	Putty.USART_BaudRate = baudrate;
	Putty.USART_WordLength = USART_WordLength_8b;
	Putty.USART_StopBits = USART_StopBits_1;
	Putty.USART_Parity = USART_Parity_No;
	Putty.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	Putty.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &Putty);
	USART_Cmd(USART1, ENABLE);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}  

void USART2_Configure(int baudrate){
    USART_InitTypeDef BlueTooth;
 
	BlueTooth.USART_BaudRate = baudrate;
	BlueTooth.USART_WordLength = USART_WordLength_8b;
	BlueTooth.USART_StopBits = USART_StopBits_1;
	BlueTooth.USART_Parity = USART_Parity_No;
	BlueTooth.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	BlueTooth.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART2, &BlueTooth);
	USART_Cmd(USART2, ENABLE);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
}

void USART3_Configure(int baudrate){
    USART_InitTypeDef WIFI;
 
	WIFI.USART_BaudRate = baudrate;
	WIFI.USART_WordLength = USART_WordLength_8b;
	WIFI.USART_StopBits = USART_StopBits_1;
	WIFI.USART_Parity = USART_Parity_No;
	WIFI.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	WIFI.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART3, &WIFI);
	USART_Cmd(USART3, ENABLE);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
}


void USART1_InterruptConfigure(){
    EXTI_InitTypeDef Putty_IR_Confiure;
	NVIC_InitTypeDef Putty_IRQ_Configure;

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource10);
 
	Putty_IR_Confiure.EXTI_Line     = EXTI_Line10;               // RX
	Putty_IR_Confiure.EXTI_LineCmd  = ENABLE;                // Enable Interrupt
	Putty_IR_Confiure.EXTI_Mode     = EXTI_Mode_Interrupt;         // Interrupt mode
	Putty_IR_Confiure.EXTI_Trigger  = EXTI_Trigger_Rising_Falling;
	EXTI_Init(&Putty_IR_Confiure);
 
	/* Enable the USART1 NVIC_init */
	Putty_IRQ_Configure.NVIC_IRQChannel                     = USART1_IRQn;
	Putty_IRQ_Configure.NVIC_IRQChannelPreemptionPriority   = 1;
	Putty_IRQ_Configure.NVIC_IRQChannelSubPriority          = 1;
	Putty_IRQ_Configure.NVIC_IRQChannelCmd                  = ENABLE;
	NVIC_Init(&Putty_IRQ_Configure);
}
void USART2_InterruptConfigure(){
	EXTI_InitTypeDef BlueTooth_IR_Configure;
    NVIC_InitTypeDef BlueTooth_IRQ_Configure;

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource3);
 
	BlueTooth_IR_Configure.EXTI_Line        = EXTI_Line3;  // RX
	BlueTooth_IR_Configure.EXTI_LineCmd     = ENABLE;   // Enable Interrupt
	BlueTooth_IR_Configure.EXTI_Mode        = EXTI_Mode_Interrupt;  // Interrupt mode
	BlueTooth_IR_Configure.EXTI_Trigger     = EXTI_Trigger_Rising_Falling;
	EXTI_Init(&BlueTooth_IR_Configure);
 

	BlueTooth_IRQ_Configure.NVIC_IRQChannel                     = USART2_IRQn;
	BlueTooth_IRQ_Configure.NVIC_IRQChannelPreemptionPriority   = 0;
	BlueTooth_IRQ_Configure.NVIC_IRQChannelSubPriority          = 1;
	BlueTooth_IRQ_Configure.NVIC_IRQChannelCmd                  = ENABLE;
	NVIC_Init(&BlueTooth_IRQ_Configure);

} 
void USART3_InterruptConfigure(){
    NVIC_InitTypeDef WIFI_IRQ_Configure;
	EXTI_InitTypeDef WIFI_IR_Configure;
 
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource11);
 
	WIFI_IR_Configure.EXTI_Line     = EXTI_Line11;  // RX
	WIFI_IR_Configure.EXTI_LineCmd  = ENABLE;   // Enable Interrupt
	WIFI_IR_Configure.EXTI_Mode     = EXTI_Mode_Interrupt;  // Interrupt mode
	WIFI_IR_Configure.EXTI_Trigger  = EXTI_Trigger_Rising_Falling;
	EXTI_Init(&WIFI_IR_Configure);
 
	WIFI_IRQ_Configure.NVIC_IRQChannel                      = USART2_IRQn;
	WIFI_IRQ_Configure.NVIC_IRQChannelPreemptionPriority    = 0;
	WIFI_IRQ_Configure.NVIC_IRQChannelSubPriority           = 0;
	WIFI_IRQ_Configure.NVIC_IRQChannelCmd                   = ENABLE;
	NVIC_Init(&WIFI_IRQ_Configure);
}

void USART1_IRQHandler(){
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        uint8_t recv_data1 = USART_ReceiveData(USART1);
        USART_SendData(USART1, recv_data1);
        while(!(USART1->SR & USART_SR_TXE)){}
        USART_SendData(USART2, recv_data1);
        while(!(USART2->SR & USART_SR_TXE)){}
        USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
}     
void USART2_IRQHandler(){
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
        uint8_t recv_data2 = USART_ReceiveData(USART2);
        USART_SendData(USART1, recv_data2);
        while(!(USART1->SR & USART_SR_TXE)){}

        USART_ClearITPendingBit(USART2,USART_IT_RXNE);
	}
}   
void USART3_IRQHandler(){
}      