#include "stm32f10x.h"                  // Device header

void timer2_delay_ms(unsigned int delay)
{
	RCC->APB1ENR= 1<<0; //timer2 clk enable
	TIM2->CR1|=1<<0; //timer 2 on CEN=1
	TIM2->CR1|=1<<7; //timer 2 ARR enable	
	TIM2->PSC=7999;  //prescale value set
	TIM2->ARR=delay; 
	//wait untill UIF flag set(timer2 flag) 	
	while(!(TIM2->SR &(1<<0))); 
	TIM2->SR &=0x0000;  //UIF flag reset(timer2 flag)	
}

int main()
{
	RCC->APB2ENR  = (1<<4);  //portc clk on
	RCC->APB2ENR  |= (1<<0);  //alter func clk on
	//pc13
	GPIOC->CRH = (3<<20);//output 50mhz
	GPIOC->CRH &= ~(1<<22);	//GPIO push pull mode
	while (1)
	{
		GPIOC->ODR ^= (1<<13);		
		timer2_delay_ms(2000);	
	}
	return 0;
}


