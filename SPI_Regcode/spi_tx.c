#include "stm32f10x.h"                  // Device header
#include "RTE_Components.h"             // Component selection
#include "RTE_Device.h"                 // Keil::Device:Startup

void spi_master_init(void);

void spi_tx(unsigned char data);

void spi_rx(void);

void spi_tx_string(unsigned char *data);

void spi_gpio_init(void);

void delay(void);

unsigned char rx_buf[50]={0};

unsigned char count=0;

int main()
{
	unsigned char msg[]="hello world";
	
	spi_gpio_init();
	
	spi_master_init();
	
	spi_tx('t');	
	
	spi_tx_string(msg);	
	
	while(1)
	{
		//spi_rx();
	}

	return 0;
}

void spi_gpio_init()
{
	//spi1:  PA4,5,6,7  with some timer alter function
	
	//PA4-nss
	//PA5-clk  (output)
	//PA6-miso (input)
	//PA7-mosi (output)
	
	//spi2:  PB12,13,14,15 without any timer alter function
	
	//1.gpioa and spi1 clk enable
	
	RCC->APB2ENR &=0x00000000; //reset
	
	RCC->APB2ENR |=(1<<2);   //porta clk enable
	RCC->APB2ENR |=(1<<12);    //spi1 clk enable
	
	//2.gpio pins config for spi1
	
	GPIOA->CRL &= 0x00000000;	//reset
	
	//sclk PA5
	GPIOA->CRL |=  3<<20;
	GPIOA->CRL |=  2<<22;
	
	//mosi PA7
	GPIOA->CRL |=  3<<28;
	GPIOA->CRL |=  2<<30;
	
}

void spi_master_init(void)
{
	SPI1->CR1 &=0x0000;  	//reset
	
	SPI1->CR1 |= 5<<3;		//  Baud rate control  fPCLK/64 for 60mhz.ie.1mbps speed
	
	SPI1->CR1 |= 1<<9;		//  Software slave management enabled
	
	SPI1->CR1 |= 1<<8;		//  SSI-Internal slave select NSS high1	
	
	//cpol and cpha both are disabled	
	
	//SPI1->CR2 |= 1<<2;		//  Hardware slave management enabled, SS output enable, SS output is enabled in master mode and when the cell is enabled. The cell cannot work in a multimaster environment
	
	SPI1->CR1 |= 1<<2;		//  Master configuration
	
	SPI1->CR1 |= 1<<6;		//	SPI enable
}

void spi_tx(unsigned char data)
{

	
	while(!((SPI1->SR)&(1<<1)));  //check tx buffer empty
	
	SPI1->DR = data;
	

}

void spi_rx()
{

	while(!((SPI1->SR)&(1<<0)));  //check rx not buffer empty
	
	rx_buf[count]=SPI1->DR;
	
	count++;
	
}

void spi_tx_string(unsigned char *data)
{
	while(*data)
	{
		spi_tx(*data++);
		
	}
}

void delay(void)
{
	unsigned int i=0;
	
	for(i=0;i<1000;i++);
	for(i=0;i<100;i++);
	
}
