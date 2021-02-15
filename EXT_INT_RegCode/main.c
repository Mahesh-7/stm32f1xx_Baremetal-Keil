#include "stm32f10x.h"                  // Device header

void delay()
{
	unsigned int i=0;
	
	for(i=0;i< 2000;i++);
	
	for(i=0;i<1000;i++);		
	
}

int main()
{
	RCC->APB2ENR = (1<<0);//afio
	
	RCC->APB2ENR |= (1<<2);//port a-pa12
	
	RCC->APB2ENR |= (1<<4);//port c
	
	//pc13 output
	
	GPIOC->CRH=(3<<20);
	
	GPIOC->CRH &= ~(3<<22);
	
	//pa12 input
	
	GPIOA->CRH &= ~(3<<16);
	
	GPIOA->CRH |= (1<<18);
	
	//EXTI pin config	
	
	EXTI->IMR=(1<<12);  //no mask
	
	EXTI->RTSR =(1<<12); //rising edge
	
	EXTI->FTSR &=~(1<<12);
	
	AFIO->EXTICR[3] &= ~(16<<0);//exti pin map-pa12
	
	NVIC->ISER[40/32] = (1<<(40%32));  //exti10_15

	while(1)
	{
			//GPIOC->ODR ^= (1<<13);
			//delay();
	}		
	
	return 0;
	
	
}


void EXTI15_10_IRQHandler()
{
	delay();//debounce
	
	if(EXTI->PR&(1<<12))
	{
		EXTI->PR =(1<<12);
	}
	
	GPIOC->ODR ^= (1<<13);
	
}