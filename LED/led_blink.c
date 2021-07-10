#include "stm32f10x.h"    // Device header

void delay( unsigned int time)
{	unsigned int i,j;
	for(i=0;i<time;i++)
	for(j=0;j<0x2AFF;j++); 
}

int main()
{	
	RCC->APB2ENR = 1<<4 ;  //port c clock enable 		
	GPIOC->CRH &= 0x00000000;  //clear all  bits 		
	
	
	GPIOC->CRH  &= ~(1 <<22) ; //config bits 00: General purpose output push-pull  	
	GPIOC->CRH  |= 1 <<20 ; //mode bits 01: Output mode, max speed 10 MHz 
	
	while(1)
	{
		GPIOC->ODR ^= 1<< 13 ;  //Toggle PC13
		delay(100);  //100ms delay
	}
	
	return 0;	
}
