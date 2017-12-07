#include <misc.h>
#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_usart.h>
#include <stm32f10x_exti.h>

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "Gamsong_usart.h"
#include "Gamsong_led.h"
#include "Gamsong_wifi.h"
#include "Gamsong_mp3.h"
#include "Gamsong_json.h"
#include "Gamsong_algorithm.h"

uint16_t USART1_READY = 0;
uint16_t USART2_READY = 0;
uint16_t USART3_READY = 0;
uint16_t UART4_READY = 0;
uint16_t UART5_READY = 0;
uint16_t start_JSON = 0;
uint16_t end_JSON = 0;

char USART1_M[1000] 	= "";
char USART2_M[1000] 	= "";
char USART3_M[1000]   	= "";
char UART4_M[1000]   	= "";
char UART5_M[1000]   	= "";
char JSON[2000] 		= "";

/*
 * 1.  기쁨 - 사랑, 밝은 행복, 활기찬  "happy"
2.  슬픔 - 이별, 잔잔한  "sad"
3.  우울 - 지친, 쓸쓸  "depressed"
4.  센치 - 그리움, 추억, 새벽, 비  "senti"
5.  설렘 - 고백, 썸 탈때 "ssum"
6.  편한 - 따뜻 "comfortable"
 */
const char*	ON 			= "on";
const char* OFF 		= "off";
const char* AT			= "AT+RST";
const char* HAPPY		= "happy";
const char* SAD			= "sad";
const char* DESP  		= "depressed";
const char* SENTI		= "senti";
const char* SSUM		= "ssum";
const char* COMF 		= "comfortable";



int USART1_INDEX = 0, USART1_SEND = 0;
int USART2_INDEX = 0, USART2_SEND = 0;
int USART3_INDEX = 0, USART3_SEND = 0;
int UART4_INDEX = 0, UART4_SEND = 0;
int UART5_INDEX = 0, UART5_SEND = 0;

int JSON_SZ = 0;
int MP3_NUM = 1;
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
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);
}

void UART4_RCC_Init(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOC, ENABLE);
}

void UART5_RCC_Init(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
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
	GPIO_InitTypeDef MP3_TX;
	GPIO_InitTypeDef MP3_RX;

	MP3_TX.GPIO_Mode  		= GPIO_Mode_AF_PP;
	MP3_TX.GPIO_Pin   		= GPIO_Pin_10;
	MP3_TX.GPIO_Speed 		= GPIO_Speed_50MHz;

	MP3_RX.GPIO_Mode  		= GPIO_Mode_IN_FLOATING;
	MP3_RX.GPIO_Pin   		= GPIO_Pin_11;


	GPIO_Init(GPIOB, &MP3_TX);
	GPIO_Init(GPIOB, &MP3_RX);
}

void UART4_GPIO_Init(){
	GPIO_InitTypeDef Wifi_TX;
	GPIO_InitTypeDef Wifi_RX;

	Wifi_TX.GPIO_Mode  		= GPIO_Mode_AF_PP;
	Wifi_TX.GPIO_Pin   		= GPIO_Pin_10;
	Wifi_TX.GPIO_Speed 		= GPIO_Speed_50MHz;

	Wifi_RX.GPIO_Mode  		= GPIO_Mode_IN_FLOATING;
	Wifi_RX.GPIO_Pin   		= GPIO_Pin_11;

	GPIO_Init(GPIOC, &Wifi_TX);
	GPIO_Init(GPIOC, &Wifi_RX);
}

void UART5_GPIO_Init(){
	GPIO_InitTypeDef Temp_TX;
	GPIO_InitTypeDef Temp_RX;

	Temp_TX.GPIO_Mode  		= GPIO_Mode_AF_PP;
	Temp_TX.GPIO_Pin   		= GPIO_Pin_12;
	Temp_TX.GPIO_Speed 		= GPIO_Speed_50MHz;

	Temp_RX.GPIO_Mode  		= GPIO_Mode_IN_FLOATING;
	Temp_RX.GPIO_Pin   		= GPIO_Pin_2;

	GPIO_Init(GPIOC, &Temp_TX);
	GPIO_Init(GPIOD, &Temp_RX);
}

void USART1_Configure(int baudrate){
	USART_InitTypeDef Putty;

	Putty.USART_BaudRate 				= baudrate;
	Putty.USART_WordLength 				= USART_WordLength_8b;
	Putty.USART_StopBits 				= USART_StopBits_1;
	Putty.USART_Parity 					= USART_Parity_No;
	Putty.USART_HardwareFlowControl 	= USART_HardwareFlowControl_None;
	Putty.USART_Mode 					= USART_Mode_Rx | USART_Mode_Tx;

	USART_DeInit(USART1);
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

	USART_DeInit(USART2);
	USART_Init(USART2, &BlueTooth);
	USART_Cmd(USART2, ENABLE);
}

void USART3_Configure(int baudrate){
	USART_InitTypeDef Mp3;

	Mp3.USART_BaudRate 					= baudrate;
	Mp3.USART_WordLength 				= USART_WordLength_8b;
	Mp3.USART_StopBits 					= USART_StopBits_1;
	Mp3.USART_Parity					= USART_Parity_No;
	Mp3.USART_HardwareFlowControl 		= USART_HardwareFlowControl_None;
	Mp3.USART_Mode 						= USART_Mode_Rx | USART_Mode_Tx;

	USART_DeInit(USART3);
	USART_Init(USART3, &Mp3);
	USART_Cmd(USART3, ENABLE);
}

void UART4_Configure(int baudrate){
	USART_InitTypeDef Wifi;

	Wifi.USART_BaudRate 				= baudrate;
	Wifi.USART_WordLength 				= USART_WordLength_8b;
	Wifi.USART_StopBits 				= USART_StopBits_1;
	Wifi.USART_Parity					= USART_Parity_No;
	Wifi.USART_HardwareFlowControl 		= USART_HardwareFlowControl_None;
	Wifi.USART_Mode 					= USART_Mode_Rx | USART_Mode_Tx;

	USART_DeInit(UART4);
	USART_Init(UART4, &Wifi);
	USART_Cmd(UART4, ENABLE);
}

void UART5_Configure(int baudrate){
	USART_InitTypeDef Temp;

	Temp.USART_BaudRate 				= baudrate;
	Temp.USART_WordLength 				= USART_WordLength_8b;
	Temp.USART_StopBits 				= USART_StopBits_1;
	Temp.USART_Parity					= USART_Parity_No;
	Temp.USART_HardwareFlowControl 		= USART_HardwareFlowControl_None;
	Temp.USART_Mode 					= USART_Mode_Rx | USART_Mode_Tx;

	USART_DeInit(UART5);
	USART_Init(UART5, &Temp);
	USART_Cmd(UART5, ENABLE);
}

void USART1_InterruptConfigure(){
	NVIC_InitTypeDef Putty_IRQ;

	Putty_IRQ.NVIC_IRQChannel                   	= USART1_IRQn;
	Putty_IRQ.NVIC_IRQChannelPreemptionPriority 	= 1;
	Putty_IRQ.NVIC_IRQChannelSubPriority        	= 0;
	Putty_IRQ.NVIC_IRQChannelCmd                	= ENABLE;

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	NVIC_Init(&Putty_IRQ);
}
void USART2_InterruptConfigure(){
	NVIC_InitTypeDef BlueTooth_IRQ;

	BlueTooth_IRQ.NVIC_IRQChannel                   = USART2_IRQn;
	BlueTooth_IRQ.NVIC_IRQChannelPreemptionPriority = 0;
	BlueTooth_IRQ.NVIC_IRQChannelSubPriority        = 2;
	BlueTooth_IRQ.NVIC_IRQChannelCmd                = ENABLE;

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	NVIC_Init(&BlueTooth_IRQ);

}
void USART3_InterruptConfigure(){
	NVIC_InitTypeDef Wifi_IRQ;

	Wifi_IRQ.NVIC_IRQChannel                      	= USART3_IRQn;
	Wifi_IRQ.NVIC_IRQChannelPreemptionPriority    	= 0;
	Wifi_IRQ.NVIC_IRQChannelSubPriority           	= 1;
	Wifi_IRQ.NVIC_IRQChannelCmd                  	= ENABLE;

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	NVIC_Init(&Wifi_IRQ);
}

void UART4_InterruptConfigure(){
	NVIC_InitTypeDef Mp3_IRQ;

	Mp3_IRQ.NVIC_IRQChannel                      	= UART4_IRQn;
	Mp3_IRQ.NVIC_IRQChannelPreemptionPriority    	= 0;
	Mp3_IRQ.NVIC_IRQChannelSubPriority           	= 0;
	Mp3_IRQ.NVIC_IRQChannelCmd                  	= ENABLE;

	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
	NVIC_Init(&Mp3_IRQ);
}

void UART5_InterruptConfigure(){
	NVIC_InitTypeDef Temp_IRQ;

	Temp_IRQ.NVIC_IRQChannel                      	= UART5_IRQn;
	Temp_IRQ.NVIC_IRQChannelPreemptionPriority    	= 0;
	Temp_IRQ.NVIC_IRQChannelSubPriority           	= 3;
	Temp_IRQ.NVIC_IRQChannelCmd                  	= ENABLE;

	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
	NVIC_Init(&Temp_IRQ);
}

void USART_Puts(USART_TypeDef* USARTx, char *s){
	while(*s){
		USART_SendData(USARTx, *s);
		while(!(USARTx->SR & USART_SR_TXE)){}
		s++;
	}
}
void USART_SendString(USART_TypeDef *USARTx, char *buffer, int buffer_SZ) {
	int i;
	for (i = 0 ; i < buffer_SZ; i++) {
		if(buffer[i] == '\r' || buffer[i] == '\n') continue;

		USART_SendData(USARTx, buffer[i]);
		while(!(USARTx->SR & USART_SR_TXE)){}
	}
	USART_Puts(USARTx,"\r\n");
}

void USART1_Handler_Method(){
	USART_SendString(USART1, USART1_M, USART1_SEND);
	/****open for input manually****/
	//USART_SendString(USART2, USART1_M, USART1_SEND);
	//USART_SendString(USART3, USART1_M, USART1_SEND);
	//USART_SendString(UART4, USART1_M, USART1_SEND);
	/*******************************/
	USART1_SEND = 0;
}

void USART2_Handler_Method(){
	USART_SendString(USART1, USART2_M, USART2_SEND);
	if(findStr("OK",USART2_M)){
		USART_Puts(USART2,"AT+BTSCAN\r\n");
	}
	if(findStr(HAPPY,USART2_M)){
		USART_Puts(USART1, "BT : happy received!\r\n");
	}
	if(findStr(SAD,USART2_M)){
		USART_Puts(USART1, "BT : sad received!\r\n");
	}
	if(findStr(DESP,USART2_M)){
		USART_Puts(USART1, "BT : depressed received!\r\n");
	}
	if(findStr(SENTI,USART2_M)){
		USART_Puts(USART1, "BT : senti received!\r\n");
	}
	if(findStr(SSUM,USART2_M)){
		USART_Puts(USART1, "BT : ssum received!\r\n");
	}
	if(findStr(COMF,USART2_M)){
		USART_Puts(USART1, "BT : comfortable received!\r\n");
	}

	USART2_SEND = 0;
}

void USART3_Handler_Method(){
	USART_Puts(USART1, "MP3 : RECEIVE NEXT COMMEND\r\n");
	USART_SendString(USART1, USART3_M, USART3_SEND);
	//mp3_send_cmd (0x01);
	//mp3_play_num(MP3_NUM++);

	USART3_SEND = 0;
}

void UART4_Handler_Method(){

	uint16_t WIFI_IS_NEXT = 0;
	USART_SendString(USART1, UART4_M, UART4_SEND);
	if(!start_JSON){
		if(findStr("{",UART4_M))
			start_JSON = 1;
	}
	else if(!end_JSON)
		jsonCopy(UART4_M, UART4_SEND);

	if(findStr("CLOSED",UART4_M)){
		end_JSON = 1;
		json_parsing();
	}

	if(compareStr("OK", UART4_M)){
		WIFI_IS_NEXT = 1;
		WIFI_STAGE++;
	}
	else if(compareStr("WIFI GOT IP", UART4_M)){
		WIFI_IS_NEXT = 1;
		WIFI_STAGE = 2;
	}
	else if(compareStr("CONNECT", UART4_M)){
		WIFI_IS_NEXT = 1;
		WIFI_STAGE = 3;
	}
	else if(compareStr("ERROR", UART4_M) || compareStr("WIFI DISCONNECT", UART4_M)){
		WIFI_IS_NEXT = 1;
		WIFI_STAGE = 0;
	}


	if(WIFI_IS_NEXT){
		WIFI_IS_NEXT = 0;
		WIFI_INIT_STAGE();
	}
	UART4_SEND = 0;
}

void UART5_Handler_Method(){
	USART_SendString(USART1, UART5_M, UART5_SEND);
	UART5_SEND = 0;
}

void USART1_IRQHandler(){
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		USART1_M[USART1_INDEX++] = USART_ReceiveData(USART1);
		if(USART1_M[USART1_INDEX-1] == '\r'){
			USART1_SEND 	= USART1_INDEX-1;
			USART1_INDEX 	= 0;
			USART1_READY 	= 1;
		}
	}
}
void USART2_IRQHandler(){
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		USART2_M[USART2_INDEX++] = USART_ReceiveData(USART2);
		if(USART2_M[USART2_INDEX-1] == '\n'){
			USART2_SEND 	= USART2_INDEX-1;
			USART2_INDEX 	= 0;
			USART2_READY 	= 1;
		}
	}
}
void USART3_IRQHandler(){
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
		USART3_M[USART3_INDEX++] = USART_ReceiveData(USART3);
		if(USART3_M[USART3_INDEX-1] == '\n') {
			USART3_SEND 	= USART3_INDEX-1;
			USART3_INDEX 	= 0;
			USART3_READY 	= 1;
		}
	}
}
void UART4_IRQHandler(){
	if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET) {
		USART_ClearITPendingBit(UART4,USART_IT_RXNE);
		UART4_M[UART4_INDEX++] = USART_ReceiveData(UART4);
		if(UART4_M[UART4_INDEX-1] == '\n') {
			UART4_SEND 		= UART4_INDEX-1;
			UART4_INDEX 	= 0;
			UART4_READY 	= 1;
		}
	}
}
void UART5_IRQHandler(){
	if (USART_GetITStatus(UART5, USART_IT_RXNE) != RESET) {
		USART_ClearITPendingBit(UART5,USART_IT_RXNE);
		UART5_M[UART5_INDEX++] = USART_ReceiveData(UART5);
		if(UART5_M[UART5_INDEX-1] == '\n') {
			UART5_SEND 		= UART5_INDEX-1;
			UART5_INDEX 	= 0;
			UART5_READY 	= 1;
		}
	}
}
