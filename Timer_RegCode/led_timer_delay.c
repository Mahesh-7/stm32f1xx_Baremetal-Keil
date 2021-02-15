#include "stm32f10x.h"                  // Device header


void delay_ms(volatile unsigned int delay)
{
			//timer delay =fosc/prec+1
			//fosc=60mhz  pre=60000-1	
			//timer2 config			
	
			unsigned int i=0;
	
			for(i=delay;i>0;i--)
			{
				TIM2->EGR = 0x0001; //reset timer2
				
				while(TIM2->CNT <1000);//1ms				
			}	
			
		

}
	
	

int main()
{

	RCC->APB2ENR  = (1<<4);  //portc clk on
	RCC->APB2ENR  |= (1<<0);  //alter func clk on
	
	//pc13
	GPIOC->CRH = (3<<20);
	GPIOC->CRH &= ~(1<<22);	
	
	//timer2 config
	
	RCC->APB1ENR = (1<<0);  //timer 2clk on	
	TIM2->PSC = 59;  //1us	
	TIM2->ARR =0xffff;  
	TIM2->CR1 = (1<<0);// timer 2 on
	
	while(1)
	{
		GPIOC->ODR ^= (1<<13);
		delay_ms(1000);   //1sec 
	}

}

