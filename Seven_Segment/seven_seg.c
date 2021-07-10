#include "stm32f10x.h"                  // Device header
#include "gpio.h"

//Seven segment values for common cathode
unsigned char seg[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};

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
	
	/*GPIO Configuration for seven segment*/
	GPIO_Config pa[6],pb[2];
	
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
	
	//PORTA Config
	for(i=0;i<6;i++)
	{
		GPIO_Config_Pin(&pa[i]);		
	}
	//PORTB Config
	for(i=0;i<2;i++)
	{
		GPIO_Config_Pin(&pb[i]);
	}
	
	

}
	

int main()
{
	unsigned char i,j,val[8];
	GPIO_Init();
	
	while(1)
	{
		
		for(i=0;i<10;i++)
		{
			for(j=0;j<8;j++)
			{
				val[j] = ((seg[i]>>j)& 1);		//convert hex to binary
			}			
			GPIO_Write_Data(PA,P8,val[0]);
			GPIO_Write_Data(PA,P9,val[1]);
			GPIO_Write_Data(PA,P10,val[2]);
			GPIO_Write_Data(PA,P11,val[3]);
			GPIO_Write_Data(PA,P12,val[4]);
			GPIO_Write_Data(PA,P15,val[5]);
			GPIO_Write_Data(PB,P3,val[6]);
			GPIO_Write_Data(PB,P4,val[7]);	
			delay(1000);
		}
		
	}
	
	return 0;
	
}