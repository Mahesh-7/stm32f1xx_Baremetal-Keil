#include "stm32f10x.h"                  // Device header
#include "RTE_Components.h"             // Component selection
#include "RTE_Device.h"                 // Keil::Device:Startup

void gpio_init(void);

void adc_init(void);	
	
unsigned short int adc_value(void);

unsigned short int adc_val=0;

int main()
{	
	gpio_init();
	
	adc_init();

	while(1)
	{
		adc_val=adc_value();
	}
	
	return 0;
}

void gpio_init()
{
		RCC->APB2ENR = (1<<2);//porta clk enable,PA0 adc0 channel,PA1 adc1
	
		RCC->APB2ENR |=(1<<0);//afio clk enable
	
		GPIOA->CRL =0x00000000;//input,analog mode	
}

void adc_init()
{
		RCC->APB2ENR |= (1<<9);//adc1 clk enable
	
		/*ADC should on before config other registeres*/
		ADC1->CR2 |=(1<<0);//ADCON	
		
		ADC1->CR2 |=(1<<1);//continuous convertion
		
		ADC1->SMPR2 &= ~(0x7<<0); //+sample time reg 1.5 cycle
	
		ADC1->SMPR2 |= (1<<0);//chanel 0 output enable
		
		/*Set Single conversion length*/
		ADC1->SQR1 &= ~(0xf<<20);	//single conversion
		
		ADC1->SQR3 &= ~(0x1f<<0); //ADC regular sequence register 3 (ADC_SQR3)	,0 channel	
	
}
	
unsigned short int adc_value()
{
		/*start conversion*/
		ADC1->CR2 |=(1<<0);//ADCON and start conversion
		
		while(!(ADC1->SR & (1<<1))); //EOC
		
		return (unsigned short int)ADC1->DR; //result
	
}
