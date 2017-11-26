#ifndef __GAMSONG_LED_H__
#define __GAMSONG_LED_H__

void LED_RCC_Init(void);        // RCC_APB2    on; (GPIO Port D)
void LED_Configure(void);       // PortD - LED on; (PD2,3,4,7)



void LED_TurnAllOn();
void LED_TurnAllOff();

void LED_TurnOn(bool LED1, bool LED2, bool LED3, bool LED4);    //LED 1,2,3,4 choose on
void LED_TurnOff(bool LED1, bool LED2, bool LED3, bool LED4);   //LED 1,2,3,4 choose off

#endif