#include "stm32l1xx.h"
#include <string.h>
#include "my_delay.h"

/* ----- Structure variable declarations ----- */
USART_InitTypeDef USART2_Init; // USART2 Object
GPIO_InitTypeDef GPIOB_LEDInit; // GPIOB LED Object
GPIO_InitTypeDef GPIOA_USART2Init; // GPIO USART2 Object

long i = 0;		

char rxdata[1];

int main(void)
{
 	/* ----- Clock Enable ----- */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE); // GPIOA Clock (RCC_AHBENR)
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE); // GPIOB Clock (RCC_AHBENR)
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); // USART2 Clock (RCC_APB1ENR)
	
	/* ----- Initializing Pins ----- */

	/* USART2 */
	GPIOA_USART2Init.GPIO_Pin = GPIO_Pin_3; // Configuring GPIOA - PA3 (RX) 
	GPIOA_USART2Init.GPIO_Mode = GPIO_Mode_AF; // Setting GPIOA - PA3 as Alternate Function	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); // Assigning USART 2 to PA3 (RX) in AF mode (GPIOA_AFRL - AFRL3[3:0])
	USART_StructInit(&USART2_Init); // Setting USART2 to default values																					
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); // USART2 Interrupt Enable - Bit 7 (TXE) (USART_SR)
	USART_Cmd(USART2, ENABLE); // USART2 Peripheral Enable - Bit 13 (UE) (USART_CR1) 
	
	/* LED - PB6 & PB7 */
	GPIOB_LEDInit.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7; // Configuring GPIOB - PB6 & PB7
	GPIOB_LEDInit.GPIO_Mode = GPIO_Mode_OUT; // Setting GPIOB - PB6 & PB7 as Output Mode

	/* ----- Initializing Peripherals ----- */
	GPIO_Init(GPIOA, &GPIOA_USART2Init); // USART2 - Pin 3 (RX)
	USART_Init(USART2,&USART2_Init); // USART2
	GPIO_Init(GPIOB, &GPIOB_LEDInit); // LED - PB6 & PB7
	
	
	
	/* ----- Data Recepetion ----- */
	while(1)
	{
		rxdata[0] = USART_ReceiveData(USART2); // Returns data in USART_DR
		while(USART_GetITStatus(USART2, USART_IT_RXNE)!= SET); // 
		
		GPIO_SetBits(GPIOB, GPIO_Pin_7);
		GPIO_ResetBits(GPIOB, GPIO_Pin_6);  	        
    
		delay(1);	
	    
		GPIO_ResetBits(GPIOB, GPIO_Pin_7);
		GPIO_SetBits(GPIOB, GPIO_Pin_6);	

		delay(1);
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
