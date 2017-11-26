#ifndef __GAMSONG_USART_H__
#define __GAMSONG_USRAT_H__


void USART1_RCC_Init(void);     // putty     RCC_APB2 on;(AFIO,GPIOA,USART1)
void USART2_RCC_Init(void);     // bluetooth RCC_APB1 on;(AFIO,GPIOA,USART2)
void USART3_RCC_Init(void);     // WIFI      RCC_APB1 on;(AFIO,GPIOB,USART3)


/******************************************************************
AFIO_MAPR
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
void USART1_GPIO_Init(void);    // putty     GPIO on;(A9,A10) - no remap
void USART2_GPIO_Init(void);    // bluetooth GPIO on;(A2,A3)  - no remap
void USART3_GPIO_Init(void);    // WIFI      GPIO on;(B10,B11)- no remap

void USART1_Configure(int Baudrate);    // putty     config;
void USART2_Configure(int Baudrate);    // bluetooth config;
void USART3_Configure(int Baudrate);    // WIFI      config;

void USART1_InterruptConfigure(void);   // putty     Interrupt(EXTI : EXTI_Line10, NVIC : USART1_IRQn, prioirty = 1,1)
void USART2_InterruptConfigure(void);   // bluetoooh Interrupt(EXTI : EXTI_Line3 , NVIC : USART2_IRQn, prioirty = 0,1)
void USART3_InterruptConfigure(void);   // WIFI      Interrupt(EXTI : EXTI_Line11, NVIC : USART3_IRQn, prioirty = 0,0)

void USART1_IRQHandler(void);           // putty     method when interrupt is working.
void USART2_IRQHandler(void);           // bluetooth method when interrupt is working.
void USART3_IRQHandler(void);           // WIFI      method when interrupt is working.

#endif