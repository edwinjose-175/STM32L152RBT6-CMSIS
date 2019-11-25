/* 	ARM Cortex M3 based Microcontroller from ST Microcontrollers - STM32L152RBT6; STM32L - DISCOVERY Board 
   	CMSIS (Cortex Microcontroller Software Interface Standard) Programming */

/* 	Programmed at National Institute of Electronics and Information Technology, Calicut - Embedded Systems Lab 
	Batch ED500 - August '19 - By Edwin Jose and Rashid P */ 

/* Program to transmit data using USART for every x seconds */

#include "stm32l1xx.h"
#include <string.h>

/* ----- Structure Variable Declarations ----- */
GPIO_InitTypeDef GPIOA_USART2Init; 	  // GPIOA USART2 Structure Variable
GPIO_InitTypeDef GPIOB_LEDInit; 	 // GPIOB LED Structure Variable
USART_InitTypeDef USART2_Init; 	    // USART2 Structure Variable
TIM_TimeBaseInitTypeDef TIM2_Init; // TIM2 Structure Variable

long i = 0, j, k = 0;		
int flag = 0;
char txdata[] = "ARM CORTEX M-3\n\r";

/* ----- TIM2 ISR ----- */
void TIM2_IRQHandler()
{ /* When TIM2_ARR overflows, this ISR is invoked, and a global variable 'flag' is SET to tell the main program that the data can be transmitted. The LEDs are used to indicate the overflow and transmission visually - GPIOB 6 & 7 (GPIOB_ODR: Bit 6 - ODR6 & Bit 7 - ODR7) */

	TIM_ClearITPendingBit(TIM2,TIM_IT_Update); // Clear TIM2 Interrupt Flag (TIM2_SR: Bit 0 - UIF)  
	flag = 1;
	GPIO_ToggleBits(GPIOB, GPIO_Pin_7);
	GPIO_ToggleBits(GPIOB, GPIO_Pin_6);	
}

int main(void)
{
 	/* ----- Clock Enable ----- */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE); 	 // GPIOB Clock (RCC_AHBENR: Bit 1 - GPIOBEN)
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE); 	// GPIOA Clock (RCC_AHBENR: Bit 0 - GPIOAEN)
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); // USART2 Clock (RCC_APB1ENR: Bit 17 - USART2EN)
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  // TIM2 Clock (RCC_APB1ENR: Bit 0 - TIM2EN)
	
	/* ----- Initializing Pins ----- */ 
	
	/* USART2 */
	GPIOA_USART2Init.GPIO_Pin = GPIO_Pin_2; 				  // Configuring GPIOA - PA2 (TX)
	GPIOA_USART2Init.GPIO_Mode = GPIO_Mode_AF;				 // Setting GPIOA - PA2 as Alternate Function (GPIOB_MODER: Bit 4&5 - MODER2[1:0])	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); /* Assigning USART2 to PA2 (TX) in AF mode (GPIOA_AFRL: Bit 8,9,10,11 - AFRL2[3:0]) 															i.e., writes AF7 - 0111 to AFRL2[3:0] - Refer to page 131 of the data sheet */
	USART_StructInit(&USART2_Init); 					  // Setting USART2 to Default Values
	USART_ITConfig(USART2, USART_IT_TXE, ENABLE); 		 // USART2 Interrupt Enable (USART_SR: Bit 7 - TXE)
	USART_Cmd(USART2, ENABLE);    					    // USART2 Peripheral Enable (USART_CR1: Bit 13 - UE)

	/* TIM2 */
	TIM2_Init.TIM_Prescaler = 500;
	TIM2_Init.TIM_Period = 2000; 			  // Auto-Reload Register (TIM2_ARR)
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); // TIM2 Interrupt Enable (TIM2_DIER: Bit 0 - UIE)
	TIM_Cmd(TIM2, ENABLE);				    // TIM2 Peripheral Enable (TIM2_CR1: Bit 0 - CEN)
	
	/* LED - PB6 & PB7 */
	GPIOB_LEDInit.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7; // Configuring GPIOB - PB6 & PB7
	GPIOB_LEDInit.GPIO_Mode = GPIO_Mode_OUT;       /* Setting GPIOB - PB6 & PB7 as Output Mode 
											 	      (GPIOB_MODER: Bit 12 & 13 - MODER6[1:0] + Bit 14 & 15 - MODER7[1:0]) */
	
	/* ----- Initializing peripherals ----- */
	GPIO_Init(GPIOB, &GPIOB_LEDInit); 	   // LED - PB6 & PB7
	GPIO_Init(GPIOA, &GPIOA_USART2Init);  // USART2 - Pin 2 (TX) 
	USART_Init(USART2,&USART2_Init); 	 // USART2
	TIM_TimeBaseInit(TIM2, &TIM2_Init); // TIM2
	
	/* ----- Data Transmission ----- */
	while(1)
    {		
		if(flag == 1) 
		{ /* Loop enters this block only when the global variable 'flag' is SET in TIM2 ISR when TIM2_ARR overflows. Data is transmitted character by character. When USART_DR is loaded with a character, it immediately transmits the data and on completion an interrupt flag is raised to indicate the end of transmission in USART_SR: Bit 6 - TC */

			int l;
			for(l = 0; l < strlen(txdata); l++)
			{
				USART_SendData(USART2, txdata[l]); // Loading USART_DR with a single character
				while(USART_GetITStatus(USART2, USART_IT_TXE)!= SET); // End of transmission
				
			} 
			flag = 0;
		}
	}

}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
