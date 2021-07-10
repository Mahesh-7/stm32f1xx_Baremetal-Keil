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

void keypad_Init()
{
	unsigned char i=0;
	PORTA_CLOCK_ENABLE();
	//GPIO pins for Keypad
	GPIO_Config pa[7];
	//PA0,PA1,PA2 and PA3 rows pins as input
	pa[0].CurrentPort = PA;
	pa[0].CurrentPin = P0;
	pa[0].PinMode = Input;
	pa[0].PinState = Input_Floating;
	pa[1].CurrentPort = PA;
	pa[1].CurrentPin = P1;
	pa[1].PinMode = Input;
	pa[1].PinState = Input_Floating;
	pa[2].CurrentPort = PA;
	pa[2].CurrentPin = P2;
	pa[2].PinMode = Input;
	pa[2].PinState = Input_Floating;
	pa[3].CurrentPort = PA;
	pa[3].CurrentPin = P3;
	pa[3].PinMode = Input;
	pa[3].PinState = Input_Floating;
	//PA4,PA5 and PA6 column pins as output
	pa[4].CurrentPort = PA;
	pa[4].CurrentPin = P4;
	pa[4].PinMode = Speed_50MHz_Output;
	pa[4].PinState = Output_PushPull;
	pa[5].CurrentPort = PA;
	pa[5].CurrentPin = P5;
	pa[5].PinMode = Speed_50MHz_Output;
	pa[5].PinState = Output_PushPull;
	pa[6].CurrentPort = PA;
	pa[6].CurrentPin = P6;
	pa[6].PinMode = Speed_50MHz_Output;
	pa[6].PinState = Output_PushPull;
	//PORTA Config
	for(i=0;i<7;i++)
	{
		GPIO_Config_Pin(&pa[i]);		
	}
}

unsigned char keyScan()
{
	//column1 keys
	GPIO_Write_Data(PA,P4,1);//C1
	GPIO_Write_Data(PA,P5,0);//C2
	GPIO_Write_Data(PA,P6,0);//C3
	
	if(GPIO_Read_Data(PA,P0)==1)
	{
		while(GPIO_Read_Data(PA,P0)==1);
		return '1';
	}
	else if(GPIO_Read_Data(PA,P1)==1)
	{
		while(GPIO_Read_Data(PA,P1)==1);
		return '4';
	}
	else if(GPIO_Read_Data(PA,P2)==1)
	{
		while(GPIO_Read_Data(PA,P2)==1);
		return '7';
	}
	else if(GPIO_Read_Data(PA,P3)==1)
	{
		while(GPIO_Read_Data(PA,P3)==1);
		return '*';
	}
	//column2 keys
	GPIO_Write_Data(PA,P4,0);//C1
	GPIO_Write_Data(PA,P5,1);//C2
	GPIO_Write_Data(PA,P6,0);//C3
	
	if(GPIO_Read_Data(PA,P0)==1)
	{
		while(GPIO_Read_Data(PA,P0)==1);
		return '2';
	}
	else if(GPIO_Read_Data(PA,P1)==1)
	{
		while(GPIO_Read_Data(PA,P1)==1);
		return '5';
	}
	else if(GPIO_Read_Data(PA,P2)==1)
	{
		while(GPIO_Read_Data(PA,P2)==1);
		return '8';
	}
	else if(GPIO_Read_Data(PA,P3)==1)
	{
		while(GPIO_Read_Data(PA,P3)==1);
		return '0';
	}
	//column3 keys
	GPIO_Write_Data(PA,P4,0);//C1
	GPIO_Write_Data(PA,P5,0);//C2
	GPIO_Write_Data(PA,P6,1);//C3
	
	if(GPIO_Read_Data(PA,P0)==1)
	{
		while(GPIO_Read_Data(PA,P0)==1);
		return '3';
	}
	else if(GPIO_Read_Data(PA,P1)==1)
	{
		while(GPIO_Read_Data(PA,P1)==1);
		return '6';
	}
	else if(GPIO_Read_Data(PA,P2)==1)
	{
		while(GPIO_Read_Data(PA,P2)==1);
		return '9';
	}
	else if(GPIO_Read_Data(PA,P3)==1)
	{
		while(GPIO_Read_Data(PA,P3)==1);
		return '#';
	}
	return 'n';
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
		//Cmd hex to bin conversion
		val[i]= (cmd>>i)&1;		
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
	//Command send to LCD
	for(i=0;i<8;i++)
	{
		//Cmd hex to bin conversion
		val[i]= (data>>i)&1;		
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
	LCD_Command(0xc0);//LCD ON
	LCD_Command(0x80);	//Set Cursor to Row0,Colun0
}

int main()
{
	unsigned char key;
	LCD_Init();
	keypad_Init();
	//LCD_Char('3');
	
	#if 1
	while(1)
	{
		key=keyScan();
		if(key!='n')
		{
			LCD_Char(key);
		}
	}
	#endif
	
	return 0;	
}
