/* 	ARM Cortex M3 based Microcontroller from ST Microcontrollers - STM32L152RBT6; STM32L - DISCOVERY Board 
   	CMSIS (Cortex Microcontroller Software Interface Standard) Programming */

/* 	Programmed at National Institute of Electronics and Information Technology, Calicut - Embedded Systems Lab 
	Batch ED500 - August '19 - By Edwin Jose and Rashid P */ 

/* Program to alternatively turn two LEDs ON and OFF when the a switch is ON */

#include "my_delay.h"
#include "stm32l1xx.h"

/* ----- Structure Variable Declarations ----- */
GPIO_InitTypeDef GPIOA_KeyInit;  // GPIOA Structure Variable
GPIO_InitTypeDef GPIOB_LEDInit; // GPIOB Stucture Varibale

int main(void)
{
	/* ----- Clock Enable ----- */
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);  // GPIOB Clock (RCC_AHBENR: Bit 0 - GPIOAEN)
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE); // GPIOB Clock (RCC_AHBENR: Bit 1 - GPIOBEN)

	/* ----- Initializing Pins ----- */
	
	/* Key - PA0 */
	GPIOA_KeyInit.GPIO_Pin = GPIO_Pin_0;     // Configuring GPIOA - PA0 
	GPIOA_KeyInit.GPIO_Mode = GPIO_Mode_IN; // Setting GPIOA - PA0 as Input Mode (GPIOA_MODER: Bit 0 & 1 - MODER0[1:0])
	
	/* LED - PB6 & PB7 */
	GPIOB_LEDInit.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7; // Configuring GPIOB - PB6 & PB7 					
	GPIOB_LEDInit.GPIO_Mode = GPIO_Mode_OUT; /* Setting GPIOB - PB6 & PB7 as Output Mode 
												(GPIOB_MODER: Bit 12 & 13 - MODER6[1:0] + Bit 14 & 15 - MODER7[1:0]) */
	
	/* ----- Initializing Peripherals ----- */
	GPIO_Init(GPIOB, &GPIOA_KeyInit);  // Key - PA0
	GPIO_Init(GPIOA, &GPIOB_LEDInit); // LED - PB6 & PB7	

  /* ----- Infinite loop ----- */
  while (1)
  {	/* If GPIOA_IDR: Bit 0 - IDR0 is Set (HIGH/1) i.e, if the switch PA0 is pressed, 
  	   toggle LEDs at GPIOB Pin 6 & 7 (GPIOB_ODR: Bit 6 - ODR6 & Bit 7 - ODR7) */

	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_7);
		GPIO_ResetBits(GPIOB, GPIO_Pin_6);  	        
    
		delay(1);	
	   
		GPIO_ResetBits(GPIOB, GPIO_Pin_7);
		GPIO_SetBits(GPIOB, GPIO_Pin_6);	

		delay(1);
	}	
	else
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_6);
		GPIO_ResetBits(GPIOB, GPIO_Pin_7);
	}	
  }
}
