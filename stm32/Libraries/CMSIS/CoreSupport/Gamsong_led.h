#ifndef __GAMSONG_LED_H__
#define __GAMSONG_LED_H__
#include <stdint.h>

void LED_RCC_Init (void);       // RCC_APB2    on; (GPIO Port D)
void LED_Configure(void);       // PortD - LED on; (PD2,3,4,7)

void LED_TurnOnAll (void);      // LED 1,2,3,4 on  ALL
void LED_TurnOffAll(void);      // LED 1,2,3,4 off ALL

void LED_TurnOn (uint16_t LED1, uint16_t LED2, uint16_t LED3, uint16_t LED4);   //LED 1,2,3,4 choose on
void LED_TurnOff(uint16_t LED1, uint16_t LED2, uint16_t LED3, uint16_t LED4);   //LED 1,2,3,4 choose off

#endif