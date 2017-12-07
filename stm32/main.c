#include <misc.h>
#include <stm32f10x.h>
#include <stm32f10x_exti.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_usart.h>
#include <string.h>
#include <math.h>

#include "lcd.h"
#include "Touch.h"
#include "Gamsong_led.h"
#include "Gamsong_usart.h"
#include "Gamsong_adc.h"
#include "Gamsong_mp3.h"
#include "Gamsong_wifi.h"

uint16_t MP3_init = 0;
int color[12]={WHITE,CYAN,BLUE,RED,MAGENTA,LGRAY,GREEN,YELLOW,BROWN,BRRED,GRAY};

void Delay_us(uint32_t us){
	if(us>1){
		uint32_t count=us*8-6;
		while(count--);
	}else{
		uint32_t count=2;
		while(count--);
	}
}

void SysInit(void) {
	RCC_DeInit();
}
void SetSysClock(void)
{
	volatile uint32_t StartUpCounter = 0, HSEStatus = 0;

	/* SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/
	/* Enable HSE */
	RCC->CR |= ((uint32_t)RCC_CR_HSEON);
	/* Wait till HSE is ready and if Time out is reached exit */
	do
	{
		HSEStatus = RCC->CR & RCC_CR_HSERDY;
		StartUpCounter++;
	} while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

	if ((RCC->CR & RCC_CR_HSERDY) != RESET)
	{
		HSEStatus = (uint32_t)0x01;
	}
	else
	{
		HSEStatus = (uint32_t)0x00;
	}

	if (HSEStatus == (uint32_t)0x01)
	{
		/* Enable Prefetch Buffer */
		FLASH->ACR |= FLASH_ACR_PRFTBE;

		/* Flash 0 wait state */
		FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
		FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_1;

		/* HCLK = SYSCLK */
		RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV2;

		/* PCLK2 = HCLK */
		RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV2;

		/* PCLK1 = HCLK */
		RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV1;

		RCC->CFGR &= (uint32_t)~(RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL);
		RCC->CFGR |= (uint32_t)( RCC_CFGR_PLLSRC_PREDIV1 | RCC_CFGR_PLLMULL5);


		RCC->CFGR2 &= (uint32_t)~(RCC_CFGR2_PREDIV2 | RCC_CFGR2_PLL2MUL |
				RCC_CFGR2_PREDIV1 | RCC_CFGR2_PREDIV1SRC);
		RCC->CFGR2 |= (uint32_t)(RCC_CFGR2_PREDIV2_DIV5 | RCC_CFGR2_PLL2MUL8 |
				RCC_CFGR2_PREDIV1SRC_PLL2 | RCC_CFGR2_PREDIV1_DIV5);

		/* Enable PLL2 */
		RCC->CR |= RCC_CR_PLL2ON;
		/* Wait till PLL2 is ready */
		while((RCC->CR & RCC_CR_PLL2RDY) == 0)
		{
		}

		/* Enable PLL */
		RCC->CR |= RCC_CR_PLLON;

		/* Wait till PLL is ready */
		while((RCC->CR & RCC_CR_PLLRDY) == 0)
		{
		}

		/* Select PLL as system clock source */
		RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
		RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;

		/* Wait till PLL is used as system clock source */
		while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08)
		{
		}
	}
	else{}
}



int main(void) {
	GPIO_InitTypeDef Btn;

	SysInit();
	SystemInit();
	//SetSysClock();


	ADC1_RCC_Init();
	ADC1_GPIO_init();
	ADC1_SENSOR_init();


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	Btn.GPIO_Mode   = GPIO_Mode_IPD;
	Btn.GPIO_Pin    = (GPIO_Pin_13);

	GPIO_Init(GPIOC, &Btn);

	LCD_Init();
	LCD_Clear(WHITE);

	LED_RCC_Init();
	LED_Configure();

	USART1_RCC_Init();
	USART1_GPIO_Init();
	USART1_Configure(115200);
	USART1_InterruptConfigure();

	USART2_RCC_Init();
	USART2_GPIO_Init();
	USART2_Configure(9600);
	USART2_InterruptConfigure();

	USART3_RCC_Init();
	USART3_GPIO_Init();
	USART3_Configure(9600);
	USART3_InterruptConfigure();
	//
	UART4_RCC_Init();
	UART4_GPIO_Init();
	UART4_Configure(115200);
	UART4_InterruptConfigure();

	while (1){
		ADC1_EXEC();
		if(!MP3_init && end_JSON){
			mp3_set_volume(15);
			Delay_us(30000);
			mp3_send_cmd(0x1);
			Delay_us(30000);
			MP3_init = 1;
		}

		if(!(GPIOC->IDR & GPIO_Pin_13)){
			mp3_send_cmd (0x01);
			Delay_us(1000000);
		}

		if(USART1_READY){
			USART1_Handler_Method();
			USART1_READY = 0;
		}
		if(USART2_READY){
			USART2_Handler_Method();
			USART2_READY = 0;
		}
		if(USART3_READY){
			USART3_Handler_Method();
			USART3_READY = 0;
		}
		if(UART4_READY){
			UART4_Handler_Method();
			UART4_READY = 0;
		}
	}
	return 0;
}
