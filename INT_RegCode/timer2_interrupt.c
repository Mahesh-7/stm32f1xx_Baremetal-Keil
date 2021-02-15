#include "stm32f10x.h"                  // Device header

	 
void TIM2_IRQHandler()
{
	//timer delay =fosc/prec+1
	//fosc=60mhz  pre=60000-1
	
	
	#if 1
	
		if(TIM2->SR & (1<<0))
		{
			TIM2->SR &=~ (1<<0);
			
			GPIOC->ODR ^= (1<<13);
		}	

	#endif	

}

int main()
{	
	
	RCC->APB1ENR = (1<<0);  //timer 2clk on
	RCC->APB2ENR  = (1<<4);  //portc clk on
		
	
	RCC->APB2ENR  |= (1<<0);  //alter func clk on
	
	//pc13
	GPIOC->CRH = (3<<20);
	GPIOC->CRH &= ~(1<<22);
	
	//timer 2 interrupt config
	
	TIM2->PSC = 59999;  //1ms  
	
	TIM2->ARR =1000;  //1s
		
	TIM2->DIER=1<<0;  //interrupt flag set
	
	NVIC->ISER[28/32]=1<<(28%32);  //interrupt set enable for timer2	
	
	TIM2->CR1 = (1<<0);// timer 2 on

	
	while(1)
	{
	
	}

}
