/* 	ARM Cortex M3 based Microcontroller from ST Microcontrollers - STM32L152RBT6; STM32L - DISCOVERY Board 
   	CMSIS (Cortex Microcontroller Software Interface Standard) Programming */

/* 	Programmed at National Institute of Electronics and Information Technology, Calicut - Embedded Systems Lab 
	Batch ED500 - August '19 - By Edwin Jose and Rashid P */ 

/* Program to transmit data using USART */

#include "stm32l1xx.h"
#include <string.h>

/* ----- Structure Variable Declarations ----- */
GPIO_InitTypeDef GPIOA_USART2Init; // GPIOA USART2 Structure Variable
USART_InitTypeDef USART2_Init;    // USART2 Structure Variable

long i = 0;		

char txdata[] = "ARM CORTEX M-3";

int main(void)
{
 	/* ----- Clock Enable ----- */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);     // GPIOA Clock (RCC_AHBENR: Bit 0 - GPIOAEN)
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); // USART2 Clock (RCC_APB1ENR: Bit 17 - USART2EN)
	
	/* ----- Initializing Pins ----- */ 
	
	/* USART2 */
	GPIOA_USART2Init.GPIO_Pin = GPIO_Pin_2; 				  // Configuring GPIOA - PA2 (TX) 
	GPIOA_USART2Init.GPIO_Mode = GPIO_Mode_AF; 				 // Setting GPIOA - PA2 as Alternate Function (GPIOB_MODER: Bit 4&5 - MODER2[1:0])
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); /* Assigning USART2 to PA2 (TX) in AF mode (GPIOA_AFRL: Bit 8,9,10,11 - AFRL2[3:0]) 															i.e., writes AF7 - 0111 to AFRL2[3:0] - Refer to page 131 of the data sheet */
	USART_Init(&USART2_Init); 							  // Setting USART2 to default values
	USART_ITConfig(USART2, USART_IT_TXE, ENABLE); 	     // USART2 Interrupt Enable (USART_SR: Bit 7 - TXE)
	USART_Cmd(USART2, ENABLE); 					        // USART2 Peripheral Enable (USART_CR1: Bit 13 - UE)

	/* ----- Initializing Peripherals ----- */
	GPIO_Init(GPIOA, &GPIOA_USART2Init); // USART2 - Pin 2 (TX)
	USART_Init(USART2,&USART2_Init);    // USART2
	
	/* ----- Data Transmission ----- */
	while(1)
  	{ /* Data is transmitted character by character. When USART_DR is loaded with a character, it immediately transmits the data and on completion an interrupt flag is raised to indicate the end of transmission in USART_SR: Bit 6 - TC */

		for(i = 0; i < strlen(txdata); i++)
		{
			USART_SendData(USART2, txdata[i]); // Loading USART_DR with a single character
			while(USART_GetITStatus(USART2, USART_IT_TXE)!= SET); // End of transmission
		}
	}
}