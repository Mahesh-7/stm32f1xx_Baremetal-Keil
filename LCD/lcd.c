#include "stm32f10x.h"                  // Device header
#include "gpio.h"

void delay( unsigned int time)
{	
	unsigned int i,j;
	for(i=0;i<time;i++)
	for(j=0;j<0x2AFF;j++); 
}


void GPIO_Init(void)
{
	unsigned char i=0;	
	/*Clk Enable for PORTA and PORTB */

	PORTA_CLOCK_ENABLE();
	PORTB_CLOCK_ENABLE();
	
	/*Data Pins for LCD */
	GPIO_Config pa[6],pb[5];
	
	pa[0].CurrentPort = PA;
	pa[0].CurrentPin = P8;
	pa[0].PinMode = Speed_50MHz_Output;
	pa[0].PinState = Output_PushPull;
	
	pa[1].CurrentPort = PA;
	pa[1].CurrentPin = P9;
	pa[1].PinMode = Speed_50MHz_Output;
	pa[1].PinState = Output_PushPull;
	
	pa[2].CurrentPort = PA;
	pa[2].CurrentPin = P10;
	pa[2].PinMode = Speed_50MHz_Output;
	pa[2].PinState = Output_PushPull;
	
	pa[3].CurrentPort = PA;
	pa[3].CurrentPin = P11;
	pa[3].PinMode = Speed_50MHz_Output;
	pa[3].PinState = Output_PushPull;
	
	pa[4].CurrentPort = PA;
	pa[4].CurrentPin = P12;
	pa[4].PinMode = Speed_50MHz_Output;
	pa[4].PinState = Output_PushPull;
	
	pa[5].CurrentPort = PA;
	pa[5].CurrentPin = P15;
	pa[5].PinMode = Speed_50MHz_Output;
	pa[5].PinState = Output_PushPull;

	
	pb[0].CurrentPort = PB;
	pb[0].CurrentPin = P3;
	pb[0].PinMode = Speed_50MHz_Output;
	pb[0].PinState = Output_PushPull;
	
	pb[1].CurrentPort = PB;
	pb[1].CurrentPin = P4;
	pb[1].PinMode = Speed_50MHz_Output;
	pb[1].PinState = Output_PushPull;
	
	/*Control Pins for LCD */
	
	pb[2].CurrentPort = PB;
	pb[2].CurrentPin = P6;
	pb[2].PinMode = Speed_50MHz_Output;
	pb[2].PinState = Output_PushPull;
	
	pb[3].CurrentPort = PB;
	pb[3].CurrentPin = P7;
	pb[3].PinMode = Speed_50MHz_Output;
	pb[3].PinState = Output_PushPull;
	
	pb[4].CurrentPort = PB;
	pb[4].CurrentPin = P8;
	pb[4].PinMode = Speed_50MHz_Output;
	pb[4].PinState = Output_PushPull;
	
	
	//PORTA Config
	for(i=0;i<6;i++)
	{
		GPIO_Config_Pin(&pa[i]);		
	}
	
	//PORTB Config
	for(i=0;i<5;i++)
	{
		GPIO_Config_Pin(&pb[i]);
	}

}

void LCD_Command(unsigned char cmd)
{
	unsigned char i,val[8];
	//RS for cmd mode
	GPIO_Write_Data(PB,P3,0);
	//RW
	GPIO_Write_Data(PB,P4,0);
	
	//Command send to LCD
	for(i=0;i<8;i++)
	{
		val[i]= (cmd>>i)&1;		//Cmd hex to bin conversion
	}
	GPIO_Write_Data(PA,P8,val[0]);
	GPIO_Write_Data(PA,P9,val[1]);
	GPIO_Write_Data(PA,P10,val[2]);
	GPIO_Write_Data(PA,P11,val[3]);
	GPIO_Write_Data(PA,P12,val[4]);
	GPIO_Write_Data(PA,P15,val[5]);
	GPIO_Write_Data(PB,P3,val[6]);
	GPIO_Write_Data(PB,P4,val[7]);	
	
	//EN on
	GPIO_Write_Data(PB,P8,1);
	delay(10);
	//EN off
	GPIO_Write_Data(PB,P8,0);
	
}

void LCD_Char(unsigned char data)
{
	unsigned char i,val[8];
	//RS for data mode
	GPIO_Write_Data(PB,P3,1);
	//RW
	GPIO_Write_Data(PB,P4,0);
	
	//DATA send to LCD
	for(i=0;i<8;i++)
	{
		val[i]= (data>>i)&1;		//Cmd hex to bin conversion
	}
	GPIO_Write_Data(PA,P8,val[0]);
	GPIO_Write_Data(PA,P9,val[1]);
	GPIO_Write_Data(PA,P10,val[2]);
	GPIO_Write_Data(PA,P11,val[3]);
	GPIO_Write_Data(PA,P12,val[4]);
	GPIO_Write_Data(PA,P15,val[5]);
	GPIO_Write_Data(PB,P3,val[6]);
	GPIO_Write_Data(PB,P4,val[7]);	
	
	//EN on
	GPIO_Write_Data(PB,P8,1);
	delay(10);
	//EN off
	GPIO_Write_Data(PB,P8,0);
	
}

void LCD_String(unsigned char * data)
{
	while(*data)
	{
		LCD_Char(*data++);
	}
	
}

void LCD_Init()
{
	GPIO_Init();
	LCD_Command(0x0C);//LCD ON
	LCD_Command(0x80);	//Set Cursor to Row0,ColuMn0
}

int main()
{
	LCD_Init();
	LCD_Char('1');
	//LCD_String((unsigned char*)"Hello");
	
	while(1)
	{
		
	}
	
	return 0;
	
}
