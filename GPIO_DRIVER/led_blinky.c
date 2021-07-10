#include "stm32f10x.h"                  // Device header
#include "gpio.h"

void delay( unsigned int time)
{	
	unsigned int i,j;
	for(i=0;i<time;i++)
	for(j=0;j<0x2AFF;j++); 
}

int main()
{
	PORTA_CLOCK_ENABLE();
	PORTC_CLOCK_ENABLE();
	
	GPIO_Config PC13,PA0;
	
	PC13.CurrentPort=PC;
	PC13.CurrentPin=P13;
	PC13.PinMode=Speed_50MHz_Output;
	PC13.PinState=Output_PushPull;
	
	GPIO_Config_Pin(&PC13);	
	
	PA0.CurrentPort=PA;
	PA0.CurrentPin=P0;
	PA0.PinMode=Input;
	PA0.PinState=Input_Floating;
	
	GPIO_Config_Pin(&PA0);	
	
	while(1)
	{
		if(GPIO_Read_Data(PA,P0) == 1)
		{
				GPIO_Write_Data(PC,P13,High);
				while(GPIO_Read_Data(PA,P0) == 1);
		}
		else
		{
				GPIO_Write_Data(PC,P13,Low);
		}
	}
	
	return 0;
}
