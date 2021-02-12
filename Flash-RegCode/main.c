#include "stm32f10x.h"                  // Device header
#include "RTE_Components.h"             // Component selection
#include "RTE_Device.h"                 // Keil::Device:Startup


/* Includes ------------------------------------------------------------------*/


#include<stdint.h>

#include<string.h>


uint32_t *data = (uint32_t *)"Hello World";

uint32_t Rx_Data[4];

char string[20];


unsigned char msg[]="flash test\r\n";

unsigned char show[50]={0};	

void delay()
{
		static unsigned int i=0;
		for(i=0;i<1000;i++);
		for(i=0;i<5000;i++);
	
}
	

void gpio_init()
{
	//clk init()
	RCC->APB2ENR &=0x00000000;//reset
	RCC->APB1ENR &=0x00000000;//reset
	
	RCC->APB2ENR |=(1<<3);  //portb clk enable
	RCC->APB2ENR |=(1<<4);  //portc clk enable
	RCC->APB1ENR |=(1<<18);//uart3 clk enable
	
	//pin config
	//pb10-tx
	//pa11-rx
	
	GPIOB->CRH &=0x00000000;//reset
	
	//tx
	GPIOB->CRH |=(3<<8);//output
	GPIOB->CRH |=(2<<10);//alter pp
	
	//rx
	GPIOB->CRH &=(~(1<<12));//input
	GPIOB->CRH |=(1<<14);//input floating
	
	GPIOC->CRH |= (1<<20); //output
	GPIOC->CRH &= ~(1<<22);
	
}


void uart_init()
{
	USART3->CR1 &=0x00000000;
	USART3->CR2 &=0x00000000;
	USART3->CR3 &=0x00000000;
	
	//baud rate config
	
	//Tx/ Rx baud rate =fclk/(16*USARTDIV)
	
	//USARTDIV=8mhz/(16*9600)=52.08
	//fclk=8mhz ; baud rate=9600; USARTDIV=(DIV_Mantissa+DIV_Fraction)=52.083
	//DIV_Mantissa=52 ; DIV_Fraction=0.083 =0.083*16=1.328 =1
	
		/* 115200 baud */
	//USART3->BRR |=(4<<4); //mantissa
	//USART3->BRR |=(5<<0); //Fraction
	
		//USARTDIV=(60mhz/2)/(16*115200)=32.552
	//fclk=60mhz ; baud rate=115200; USARTDIV=(DIV_Mantissa+DIV_Fraction)=32.552
	//DIV_Mantissa=32 ; DIV_Fraction=0.552 =0.552*16=8.832 =9
	
			//USARTDIV=(60mhz/2)/(16*9600)=390.625
	//fclk=60mhz ; baud rate=115200; USARTDIV=(DIV_Mantissa+DIV_Fraction)=390.625
	//DIV_Mantissa=390 ; DIV_Fraction=0.625 =0.625*16=10
	
	USART3->BRR &=0x00000000;
	USART3->BRR |=(16<<4); //mantissa
	USART3->BRR |=(4<<0); //Fraction
	//USART3->BRR=0x104; //115200

	

	
	//USART_STD_BAUD_9600;
	//USART_HW_FLOW_CTRL_NONE;
	//USART_MODE_ONLY_TX;
	//USART_STOPBITS_1;
	//USART_WORDLEN_8BITS;
	//USART_PARITY_DISABLE;
	
	USART3->CR1 |=(1<<3); //tx enable
	USART3->CR1 |=(1<<13); //uart enable
	
}



void uart_tx(unsigned char data)
{
	
	
	while( !(USART3->SR & (1<<7)));  //tx buffer empty check
	
	USART3->DR=data;
	
	//test
	//while( !(USART3->SR & (1<<6)));//tx complete check;

}
	
void uart_tx_string(unsigned char *data)
{

	while(*data)
	{
		uart_tx(*data++); delay();
	
	}
	
	while( !(USART3->SR & (1<<6)));//tx complete check;
}




void Flash_Write_Data(unsigned int address , unsigned int *wt_data);

void Flash_Read_Data(unsigned int address , unsigned int *rd_data);	
	
void Convert_To_Str(unsigned int* src,char * des);


int main(void)
{
			
	gpio_init();
	
	uart_init();
	
	uart_tx_string(msg);


  //Flash_Write_Data(0x0801FC00 , data);

	//Flash_Read_Data(0x0801FC00, Rx_Data);	
	
  //Convert_To_Str(Rx_Data, string);
	
	sprintf((char*)show,(const char*)" flash_datas: %s\r\n",string);			

	uart_tx_string(show);	

  while (1)
  {

  }

}


void Flash_Write_Data(unsigned int address , unsigned int *wt_data)
{
	
	
}

void Flash_Read_Data(unsigned int address , unsigned int *rd_data)
{
	
	
}
	
void Convert_To_Str(unsigned int* src,char * des)
{
	
	
}
