#include "stm32f10x.h"                  // Device header

void gpio_init();
	
void compare_init();


int main()
{
	gpio_init();
	
	compare_init();
	
	while(1)
	{
		#if 0
		if(TIM2->SR &(1<<1))
		{
			TIM2->SR &=~(1<<1);//compare flag reset
		}
		#endif
	}
	
	return 0;
}


void gpio_init()
{
		RCC->APB2ENR =(1<<0); //afio clk enable
	
		RCC->APB2ENR |= (1<<2);//porta clk enable,
	
		GPIOA->CRL =0x00000000;//reset
	
		//pa0
	
		GPIOA->CRL |=(3<<0);//output
	
		GPIOA->CRL |=(2<<2);//afio output	
}

/*******Set timer 2 Period**************
  PWM Frequency                 = 1000Hz
  Timer Clock                   = 8mhz = 8000000 Hz
  Time Period Required          = (1/1000Hz) = 0.001 Seconds=1ms
	timer clk Formula							= fosc/(pre+1)
  Prescalar + 1                 = 8000000/1000hz)
  Prescalar                     = 7999  
	Auto_Reload										= 2000
***********************************************/

void compare_init()
{
	/* Enabling clock for Timer 2*/
	RCC->APB1ENR = (1<<0);//tim2 clk enable

  TIM2->ARR = 2000;   //for 2s               
  TIM2->PSC = 7999;                       

	/* Output Compare Mode, ENABLE Preload*/
  TIM2->CCMR1  =  TIM_CCMR1_OC1PE |(3<<4); //oc1m-toggle ,OC1PE: Output compare 1 preload enable

  //TIM2->EGR |= TIM_EGR_UG |TIM_EGR_CC1G ;  

  /*CC2E : channel 1 enabled; polarity : active low*/ 

  TIM2->CCER = 0;  //compare off
	
	TIM2->CCR1 = 1000;//compare val

  /* Auto Reload Enable */
  TIM2->CR1 |= TIM_CR1_ARPE;//buffer enable
	
	TIM2->CCER |= TIM_CCER_CC1E;//compare enable

	/* Enable Timer Counter */
  TIM2->CR1 |= TIM_CR1_CEN;   //timer on  

}

