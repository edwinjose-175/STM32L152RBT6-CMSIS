/*  ARM Cortex M3 based Microcontroller from ST Microcontrollers - STM32L152RBT6; STM32L - DISCOVERY Board  
    CMSIS (Cortex Microcontroller Software Interface Standard) Programming */

/*  Programmed at National Institute of Electronics and Information Technology, Calicut - Embedded Systems Lab
    Batch ED500 - August '19 - By Edwin Jose and Rashid P */ 

/* Program to alternatively turn two LEDs ON and OFF */

#include "my_delay.h"
#include "stm32l1xx.h"

/* ----- Structure Variable Declarations ----- */
GPIO_InitTypeDef GPIOB_LEDInit; // GPIOB Structure Variable

int main(void)
{
 /* ----- Clock Enable ----- */
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE); // GPIOB Clock (RCC_AHBENR: Bit 1 - GPIOBEN)

	/* ----- Initializing Pins ----- */
	
	/* LED - PB6 & PB7 */
	GPIOB_LEDInit.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7; // Configuring GPIOB - PB6 & PB7 								
	GPIOB_LEDInit.GPIO_Mode = GPIO_Mode_OUT; 	   // Setting GPIOB - PB6 & PB7 as Output Mode (GPIOB_MODER: Bit 12 & 13 - MODER6[1:0] + Bit 14 & 15 - MODER7[1:0])	 				
		
	/* ----- Initializing Peripherals ----- */
	GPIO_Init(GPIOB, &GPIOB_LEDInit); // GPIOB LED - PB6 & PB7	
	
	while (1)
  { /* Toggling LEDs at Pin 6 and 7 with software delay (GPIOB_ODR: Bit 6 - ODR6 & Bit 7 - ODR7) */

	  GPIO_SetBits(GPIOB, GPIO_Pin_7);
	  GPIO_ResetBits(GPIOB, GPIO_Pin_6);  	        
    
		delay(1);	
	    
		GPIO_ResetBits(GPIOB, GPIO_Pin_7);
		GPIO_SetBits(GPIOB, GPIO_Pin_6);	

		delay(1);
			
  }
}
