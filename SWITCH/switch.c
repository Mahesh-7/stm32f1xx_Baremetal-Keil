#include "stm32f10x.h"    // Device header

void delay( unsigned int time)
{	unsigned int i,j;
	for(i=0;i<time;i++)
	for(j=0;j<0x2AFF;j++); 
}

int main()
{	
	RCC->APB2ENR = 1<<4 ;  //port C clock enable 
	RCC->APB2ENR |= 1<<2 ;  //port A clock enable 		
	GPIOC->CRH &= 0x00000000;  //clear all  bits 
	
	/*PA0 as input*/
	GPIOA->CRL  |=( 1 <<2) ; //config bits 01: Floating input (reset state) 	
	GPIOA->CRL  &= ~(3 <<0) ; //mode bits 00: Input mode (reset state) 
	
	/*PC13 as output */		
	GPIOC->CRH  &= ~(1 <<22) ; //config bits 00: General purpose output push-pull  	
	GPIOC->CRH  |= 1 <<20 ; //mode bits 01: Output mode, max speed 10 MHz 
	while(1)
	{
			if((GPIOA->IDR & (1<<0) )==1 )
			{
				//delay(100);  //100ms delay for debounce
				GPIOC->ODR |= 1<< 13 ;  //on PC13
				while((GPIOA->IDR & (1<<0) )==1);//to avoid repeat process used blocking statement
			}
			else
			{
				GPIOC->ODR &= ~(1<< 13) ;  //off PC13
			}
	}
	return 0;	
}


