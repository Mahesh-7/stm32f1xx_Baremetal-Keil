#include "stm32f10x.h"                  // Device header
#include "RTE_Components.h"             // Component selection
#include "RTE_Device.h"                 // Keil::Device:Startup

#include<string.h>

void i2c_gpio_init(void);
void i2c_init(void);
void i2c_slave_init(void);
void i2c_start(void);
void i2c_slave_address(unsigned char data);
void i2c_write(unsigned char data);
void i2c_clearAddr(void);
void i2c_write_string(unsigned char *data);
unsigned char read();
void i2c_stop(void);

void delay(void);

unsigned char * msg=(unsigned char *) "hello embedded world";

unsigned short int dummy_read=0x0000;

int main(void)
{
	
	i2c_gpio_init();//delay();		
	
	i2c_init();//delay();		
	
	i2c_start();

	i2c_slave_address(0xA0);
	
	i2c_write_string(msg);

	i2c_stop();

	return 0;
}

void delay(void)
{
	unsigned int i;
	for(i=0;i<1000;i++);
	for(i=0;i<1000;i++);
}

void i2c_gpio_init(void)
{
	//i2c and port clk enable
	RCC->APB1ENR &= 0x00000000;
	RCC->APB2ENR &= 0x00000000;
	
	RCC->APB1ENR |= (1<<21); //enable i2c clk
	RCC->APB2ENR |= (1<<3);  //enable portb clk
	//RCC->APB2ENR |=(1<<0);  //alternate function clk enable
	
	GPIOB->CRL &=0x00000000;
	
	//scl	
	GPIOB->CRL |= (3<<24);  //pb6-output
	GPIOB->CRL |= (3<<26);  //pb6-alterante open drain

	//sda
	GPIOB->CRL |= (3<<28);  //pb7-output
	GPIOB->CRL |= (3<<30);  //pb7-alterante open drain
	
}



void i2c_init(void)
{
	//i2c master mode
	I2C1->CR1 &=0x0000;
	I2C1->CR2 &=0x0000;
	
	I2C1->CR1 |= (1<<10);  //ack enable
	
	I2C1->CR2 |= 8<<0;  //freq :8mhz
	
	I2C1->CCR =0x28;  //100khz
	
	I2C1->TRISE &=0x0000;
	I2C1->TRISE |=9;  //scal rise time config
	
	I2C1->CR1 |= (1<<0); //i2c peripheral enable
}


void i2c_start()
{
	//1.i2c start
	I2C1->CR1 |= (1<<8); 
	
	while( ! (I2C1->SR1 & (1<<0)));
	dummy_read=I2C1->SR1;
	(void)dummy_read;
	
}

void i2c_slave_address(unsigned char data)
{
	//2.i2c slave address+write
	I2C1->DR = data;//delay();	
	
	i2c_clearAddr();
}

void i2c_clearAddr()
{
		
	//2.a) check addr clear
	while( ! (I2C1->SR1 & (1<<1)));
	dummy_read=I2C1->SR1;
	dummy_read=I2C1->SR2;
	(void)dummy_read;
	
}
void i2c_write_string(unsigned char *data)
{
	unsigned char len= (unsigned char ) strlen((char*)data);
	
	//3. before i2c master write check txe empty
	while(len>0)
	{
		while( ! (I2C1->SR1 & (1<<7)));//check TXE flag
		I2C1->DR = *data++; //delay();		
		len--;
	}
	
}

void i2c_slave_init(void)
{
		//i2c master mode
	I2C1->CR1 &=0x0000;
	I2C1->CR2 &=0x0000;
	
	I2C1->CR1 |= (1<<10);  //ack enable
	
	I2C1->OAR1 =0xA0<<1;//slave address 0xA0
	
	I2C1->CR1 |= (1<<0); //i2c peripheral enable
	
}

unsigned char read()
{
	//check addr clear
	while( ! (I2C1->SR1 & (1<<1)));
	dummy_read=I2C1->SR1;
	dummy_read=I2C1->SR2;
	while( ! (I2C1->SR1 & (1<<6)));//check RxNE
	return I2C1->DR;
}

void i2c_stop()
{
	//before stop check txe and btf are set
	while( ! (I2C1->SR1 & (1<<7)));
	while( ! (I2C1->SR1 & (1<<2)));
	
	//4.i2c stop
	I2C1->CR1 |= (1<<9);
	
}



