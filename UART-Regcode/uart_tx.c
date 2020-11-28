#include "stm32f10x.h"                  // Device header
#include "RTE_Components.h"             // Component selection
#include "RTE_Device.h"                 // Keil::Device:Startup

void gpio_init();
	
void uart_init();
	
void uart_tx(unsigned char data);
	
void uart_tx_string(unsigned char *data);

int main()
{
	unsigned char msg[]="hello";
	
	gpio_init();
	
	uart_init();
	
	uart_tx('a');
	
	uart_tx_string(msg);
	
	while(1);
}

void gpio_init()
{
	//clk init()
	RCC->APB2ENR &=0x00000000;//reset

	RCC->APB2ENR |=(1<<2);  //porta clk enable
	RCC->APB2ENR |=(1<<14);//uart clk enable
	
	//pin config
	//pa9-tx
	//pa10-rx
	
	GPIOA->CRH &=0x00000000;//reset
	
	//tx
	GPIOA->CRH |=(3<<4);//output
	GPIOA->CRH |=(2<<6);//alter pp
	
	//rx
	GPIOA->CRH &=~(1<<8);//input
	GPIOA->CRH |=(1<<10);//input floating
	
}
	
void uart_init()
{
	USART1->CR1 &=0x00000000;
	USART1->CR2 &=0x00000000;
	USART1->CR3 &=0x00000000;
	
	//baud rate config
	
	//Tx/ Rx baud rate =fclk/(16*USARTDIV)
	//fclk=8mhz ; baud rate=9600; USARTDIV=DIV_Mantissa+DIV_Fraction=52.083
	
	USART1->BRR &=0x00000000;
	USART1->BRR |=(52<<4); //mantissa
	USART1->BRR |=(1<<0); //Fraction
	
	//USART_STD_BAUD_9600;
	//USART_HW_FLOW_CTRL_NONE;
	//USART_MODE_ONLY_TX;
	//USART_STOPBITS_1;
	//USART_WORDLEN_8BITS;
	//USART_PARITY_DISABLE;
	
	USART1->CR1 |=(1<<3); //tx enable
	USART1->CR1 |=(1<<13); //uart enable
	
}
	
void uart_tx(unsigned char data)
{
	
	
	while( !(USART1->SR & (1<<7)));  //tx buffer empty check
	
	USART1->DR=data;
	

}
	
void uart_tx_string(unsigned char *data)
{
	while(*data)
	{
		uart_tx(*data++);
	}
	
	while( !(USART1->SR & (1<<6)));//tx complete check;
}





