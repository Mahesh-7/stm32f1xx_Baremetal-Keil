#include "stm32f10x.h"                  // Device header
#include "RTE_Components.h"             // Component selection
#include "RTE_Device.h"                 // Keil::Device:Startup


/* Includes ------------------------------------------------------------------*/

#include<stdint.h>

#include<string.h>


/*  FLASH EXTRACTIONS 1024*10PAGES =1KB*10=10KB */

#define PAGE_118 0x0801D400
#define PAGE_119 0x0801D800
#define PAGE_120 0x0801DC00
#define PAGE_121 0x0801E000
#define PAGE_122 0x0801E400
#define PAGE_123 0x0801E800
#define PAGE_124 0x0801EC00
#define PAGE_125 0x0801F400
#define PAGE_126 0x0801F800
#define PAGE_127 0x0801FC00

uint32_t *data = (uint32_t *)"embedded tech";

uint32_t Rx_Data[4];

char string[20];

unsigned char msg[]="flash test\r\n";

unsigned char show[50]={0};	

void Flash_Write(uint32_t address , uint32_t *wt_data);

void Flash_Read(uint32_t address,volatile uint32_t *data);

void convert_to_string(char *str ,uint32_t *data);

void flash_erase(unsigned int address);

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
	
	//USARTDIV=(60mhz/2)/(16*115200)=32.552
	//fclk=60mhz ; baud rate=115200; USARTDIV=(DIV_Mantissa+DIV_Fraction)=32.552
	//DIV_Mantissa=32 ; DIV_Fraction=0.552 =0.552*16=8.832 =9

	
	USART3->BRR &=0x00000000;
	USART3->BRR |=(16<<4); //mantissa
	USART3->BRR |=(4<<0); //Fraction

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

int main(void)
{
			
	gpio_init();
	
	uart_init();
	
	uart_tx_string(msg);

  Flash_Write(PAGE_118, data);

	Flash_Read(PAGE_118,	Rx_Data);

	convert_to_string(string ,Rx_Data);
	
	sprintf((char*)show,(const char*)" flash_datas: %s\r\n",string);			

	uart_tx_string(show);	

  while (1)
  {

  }

}
void	flash_unlock()
{

		/*authorize flass access*/
		FLASH->KEYR=0x45670123;
		FLASH->KEYR=0xCDEF89AB;

}
	
void	flash_lock()
{
	
	FLASH->CR = (1<<7);
}

void flash_erase(unsigned int address)
{	
		while(FLASH->SR & (1<<0));//check busy	
	
		FLASH->CR |= (1<<1);  //Page erase
		
		FLASH->AR =address;
	
		FLASH->CR |= (1<<6);  //STRT: Start erase
	
		while(FLASH->SR & (1<<0));//check busy	
	
	
}

void Flash_Write(unsigned int address , unsigned int *wt_data)
{	
	static unsigned int idx=0;
		
	int len=(strlen((char *)wt_data)/4) + ((strlen((char *)wt_data) % 4) != 0);
	int index=0;
	
	FLASH->CR = 0x00000000;
	
	flash_unlock(); //write access
	
	flash_erase(address);
	
	FLASH->CR = 0x00000000;  //reset erase
	
	while(FLASH->SR & (1<<0));//check busy	
	FLASH->CR |= (1<<0);//enable programming bit PG
	
	while(index<len)
	{
		for(idx=0;idx<2	;idx++) //min 2 byte have to write
		{
			*(unsigned short int*)address = (unsigned short int)(wt_data[index]>>(16*idx));//little endian
			address+=2;
			while(FLASH->SR & (1<<0));//check busy			
		}		
		index++;

	}	
	
	flash_lock();
	
}



void Flash_Read(uint32_t address,volatile uint32_t *data)
{
	
	while(1)
	{
		*data=*(volatile uint32_t *)address;
		if(*data == 0xffffffff)
		{			
			*data ='\0';
			break;
		}
		address+=4;
		data++;		

	}

}

void convert_to_string(char *str ,uint32_t *data)
{
	
	int len=((strlen((char*)data)/4)+((strlen((char*)data)%4) != 0))*4;
	
	static int idx=0;
	
	for(idx=0;idx<len;idx++)
	{		
		str[idx]= (data[idx/4])>>(8*(idx%4));
	}
	
	
}



