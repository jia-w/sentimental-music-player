#ifndef __GAMSONG_USART_H__
#define __GAMSONG_USRAT_H__

#include <stdint.h>
#include <stm32f10x.h>

extern uint16_t USART1_READY;
extern uint16_t USART2_READY;
extern uint16_t USART3_READY;
extern uint16_t UART4_READY;
extern uint16_t UART5_READY;

extern char JSON[2000];
extern int JSON_SZ;

void USART1_RCC_Init(void);     // putty     RCC on;(AFIO,GPIOA,USART1)
void USART2_RCC_Init(void);     // bluetooth RCC on;(AFIO,GPIOA,USART2)
void USART3_RCC_Init(void);     // MP3       RCC on;(AFIO,GPIOB,USART3)
void UART4_RCC_Init(void);      // WIFI      RCC on;(AFIO,GPIOC,UART4)
void UART5_RCC_Init(void);      //           RCC on;(AFIO,GPIOC,GPIOD,UART5)

/******************************************************************
AFIO_MAPR
UART5_REMAP
0: No remap (TX/PC12, RX/PD2)

UART4_REMAP
0: No remap (TX/PC10, RX/PC11)

USART3_REMAP
00: No remap (TX/PB10, RX/PB11, CK/PB12, CTS/PB13, RTS/PB14)
01: Partial remap (TX/PC10, RX/PC11, CK/PC12, CTS/PB13, RTS/PB14)
10: not used
11: Full remap (TX/PD8, RX/PD9, CK/PD10, CTS/PD11, RTS/PD12)

USART2_REMAP
0: No remap (CTS/PA0, RTS/PA1, TX/PA2, RX/PA3, CK/PA4)
1: Remap (CTS/PD3, RTS/PD4, TX/PD5, RX/PD6, CK/PD7)

USART1_REMAP
0: No remap (TX/PA9, RX/PA10)
1: Remap (TX/PB6, RX/PB7)
*******************************************************************/
void USART1_GPIO_Init(void);    // putty        GPIO on;(A9,A10) - no remap
void USART2_GPIO_Init(void);    // bluetooth    GPIO on;(A2,A3)  - no remap
void USART3_GPIO_Init(void);    // MP3          GPIO on;(B10,B11)- no remap
void UART4_GPIO_Init(void);     // WIFI         GPIO on;(C10,C11)- no remap
void UART5_GPIO_Init(void);     //              GPIO on;(C12,D2) - no remap

void USART1_Configure(int Baudrate);    // putty     config;
void USART2_Configure(int Baudrate);    // bluetooth config;
void USART3_Configure(int Baudrate);    // MP3       config;
void UART4_Configure(int Baudrate);     // WIFI      config;
void UART5_Configure(int Baudrate);     //           config;

void USART1_InterruptConfigure(void);   // putty     Interrupt(NVIC : USART1_IRQn, prioirty = 1,1)
void USART2_InterruptConfigure(void);   // bluetoooh Interrupt(NVIC : USART2_IRQn, prioirty = 0,2)
void USART3_InterruptConfigure(void);   // MP3       Interrupt(NVIC : USART3_IRQn, prioirty = 0,1)
void UART4_InterruptConfigure(void);    // WIFI      Interrupt(NVIC : UART4_IRQn, prioirty = 0,0)
void UART5_InterruptConfigure(void);    //           Interrupt(NVIC : UART4_IRQn, prioirty = 0,3)

void USART_Puts(USART_TypeDef* USARTx, char *s);
void USART_SendString(USART_TypeDef *USARTx, char *buffer, int buffer_SZ);

void USART1_Handler_Method(void);
void USART2_Handler_Method(void);
void USART3_Handler_Method(void);
void UART4_Handler_Method(void);
void UART5_Handler_Method(void);

void USART1_IRQHandler(void);           // putty     method when interrupt is working.
void USART2_IRQHandler(void);           // bluetooth method when interrupt is working.
void USART3_IRQHandler(void);           // MP3       method when interrupt is working.
void UART4_IRQHandler(void);            // WIFI      method when interrupt is working.
void UART5_IRQHandler(void);            //           method when interrupt is working.

#endif
