/* 	ARM Cortex M3 based Microcontroller from ST Microcontrollers - STM32L152RBT6; STM32L - DISCOVERY Board 
   	CMSIS (Cortex Microcontroller Software Interface Standard) Programming */

/* 	Programmed at National Institute of Electronics and Information Technology, Calicut - Embedded Systems Lab 
	Batch ED500 - August '19 - By Edwin Jose and Rashid P */ 

/* Program to alternatively turn two LEDs ON and OFF when a switch is ON using External Interrupt ISR*/


#include "stm32l1xx.h"

/* ----- Structure Variable Declarations ----- */
GPIO_InitTypeDef GPIOB_LEDInit; 	// GPIOB Structure Variable
TIM_TimeBaseInitTypeDef TIM2_Init; // TIM2 Structure Variable

long i = 0;		

int main(void)
{
 	/* ----- Clock Enable ----- */
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);   // GPIOB Clock (RCC_AHBENR: Bit 1 - GPIOBEN)
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // TIM2 Clock (RCC_APB1ENR: Bit 0 - TIM2EN)

	/* ----- Initializing Pins ----- */
	
	/* LED - PB6 & PB7 */
	GPIOB_LEDInit.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7; // Configuring GPIOB - PB6 & PB7 						
	GPIOB_LEDInit.GPIO_Mode = GPIO_Mode_OUT; 	   /* Setting GPIOB - PB6 & PB7 as Output Mode 
											          (GPIOB_MODER: Bit 12 & 13 - MODER6[1:0] + Bit 14 & 15 - MODER7[1:0]) */						
	
	/* TIM2 */
	TIM2_Init.TIM_Prescaler = 500;
	TIM2_Init.TIM_Period = 2000; 			  // Auto-Reload Register (TIM2_ARR)
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); // TIM2 Interrupt Enable (TIM2_DIER: Bit 0 - UIE)
	TIM_Cmd(TIM2, ENABLE); 					// TIM2 Peripheral Enable (TIM2_CR1: Bit 0 - CEN)
		
	/* ----- Initializing Peripherals ----- */
	GPIO_Init(GPIOB, &GPIOB_LEDInit); 	 // LED - PB6 & PB7
	TIM_TimeBaseInit(TIM2, &TIM2_Init); // TIM2

  	while (1)
  	{ /* When TIM2_ARR overflows, TIM2_SR: Bit 0 - UIF is Set (HIGH/1), and when that happens the while loop enters the if statement,
	     and the IT Bit is first Reset (LOW/0) and GPIOB Pin 6 & 7 (GPIOB_ODR: Bit 6 - ODR6 & Bit 7 - ODR7) are toggled */ 
	
		if(TIM_GetITStatus(TIM2, TIM_IT_Update)!= RESET)
		{	
			TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
			
			GPIO_ToggleBits(GPIOB, GPIO_Pin_7);
		 	GPIO_ToggleBits(GPIOB, GPIO_Pin_6);	
		}	
	}
}