#include "stm32f10x.h"                  // Device header

void gpio_init();
	
void capture_init();


int main()
{
	unsigned short int dummy_read=0;
	
	gpio_init();
	
	capture_init();
	
	while(1)
	{
		#if 0 //dont use if read TIM2->CCR1 register
		if(TIM2->SR &(1<<1))
		{
			TIM2->SR &=~(1<<1);//capture flag reset
		}
		#endif
		
		dummy_read=TIM2->CCR1;
	}
	
	return 0;
}


void gpio_init()
{
		RCC->APB2ENR =(1<<0); //afio clk enable
	
		RCC->APB2ENR |= (1<<2);//porta clk enable,
	
		GPIOA->CRL =0x00000000;//reset
	
		//pa0
	
		GPIOA->CRL &=~(3<<0);//input
	
		GPIOA->CRL |=(1<<2);//input	floating
}

/*******Set timer 2 Period  **************
  PWM Frequency                 = 1000Hz
  Timer Clock                   = 8mhz = 8000000 Hz
  Time Period Required          = (1/1000Hz) = 0.001 Seconds=1ms
	timer clk Formula							= fosc/(pre+1)
  Prescalar + 1                 = 8000000/1000hz)
  Prescalar                     = 7999  
	Auto_Reload										= 2000
***********************************************/

void capture_init()
{
	/* Enabling clock for Timer 2*/
	RCC->APB1ENR = (1<<0);//tim2 clk enable

  TIM2->ARR = 2000;   //for 2s               
  TIM2->PSC = 7999;                       

	/* Capture Mode, ENABLE Preload*/
  TIM2->CCMR1  =  TIM_CCMR1_CC1S_0 ; 

  //TIM2->EGR |= TIM_EGR_UG |TIM_EGR_CC1G ;  

  /*CC2E : channel 1 enabled; polarity : active low*/ 

  TIM2->CCER = 0;  // off
	
	TIM2->CCR1 = 0; 

  /* Auto Reload Enable */
  TIM2->CR1 |= TIM_CR1_ARPE;//buffer enable
	
	TIM2->CCER |= TIM_CCER_CC1E;//enable

	/* Enable Timer Counter */
  TIM2->CR1 |= TIM_CR1_CEN;   //timer on  

}

