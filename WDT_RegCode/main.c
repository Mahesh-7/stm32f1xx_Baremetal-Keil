#include "stm32f10x.h"                  // Device header
#include "RTE_Components.h"             // Component selection
#include "RTE_Device.h"                 // Keil::Device:Startup
#include "WDG.h"


/**********************prescale value set ****************

	timer clk=1000hz=1ms; fosc=60mhz; prescalar ? 
	
	 timer clk				=	fosc/(prescaler+1);
	 1000							=	60000000/(prescaler+1);
	(prescaler+1)			=	60000000/1000;
	 prescaler				=	60000-1;
	 prescaler				=	59999;
	 
*********************************************************/

void timer2_delay_ms(unsigned int delay)
{
	RCC->APB1ENR |= 1<<0; //timer2 clk enable
	
	TIM2->PSC=59999;  //prescale value set

	TIM2->ARR=delay; //auto reload reg to compare with counter reg  *multiply with prescaler timer ( 1ms*1000=1 sec)

	TIM2->CR1|=1<<0; //timer 2 on CEN=1
	
	while(!(TIM2->SR &(1<<0))); //wait untill UIF flag set(timer2 flag)
	TIM2->SR &=0x0000;  //UIF flag reset(timer2 flag)
	
}

void Gpio_init(void)
{
	RCC->APB2ENR |= (1<<4); //portc clk enable
	
	GPIOC->CRH |= (3<<20);  //output
	
	GPIOC->CRH &= ~(1<<22);	//GPIOA pushpull
	
}

void Gpio_Toggle(unsigned char pin)
{
	GPIOC->ODR ^= (1<<pin);
}

int main()
{
	WDG_Init();
	
	Gpio_init();
	
	Gpio_Toggle(13);
	
	timer2_delay_ms(100);	

	while (1)
	{
		WDG_Sync();	//should reset within 1ms to 50ms ,WDT will otherwise reset controller
		
		timer2_delay_ms(45);
	}
	
return 0;
	
}