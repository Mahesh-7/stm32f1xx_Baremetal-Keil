#include "stm32f10x.h"                  // Device header
#include "RTE_Components.h"             // Component selection
#include "RTE_Device.h"                 // Keil::Device:Startup
#include<string.h>

typedef struct RTC_Handle
{	
	unsigned int Day;
	unsigned int Month;
	unsigned int Year;
	
	unsigned int Hour;
	unsigned int Minutes;
	unsigned int Second;	
	
}RTC_Handle;


//RTC function variables

static unsigned char Month_Days[]=	{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

RTC_Handle SetTime={0};
RTC_Handle GetTime={0};

RTC_Handle *GetTime1=0;

unsigned char msg[]="hello\r\n";

unsigned char show[50]={0};	

unsigned long int i=0;

unsigned long int count_seconds=0;

unsigned long int temp_days=0; 

//RTC functions

void Make_Time();	
	
RTC_Handle*Break_Time();

void rtc_set_time(unsigned int date,unsigned int month,unsigned int year,unsigned int hour,unsigned int min,unsigned int sec);

RTC_Handle* rtc_get_time();

unsigned char check_leap(unsigned int year);

void gpio_init();
	
void uart_init();
	
void uart_tx(unsigned char data);
	
void uart_tx_string(unsigned char *data);

void delay(void);

void rtc_init();

void Wait_Finished();

void Clear_Sync();

void Wait_Sync();

void rtc_prescaler(unsigned int pre);

void RTC_set_Count(unsigned long int count_seconds);

unsigned long int  RTC_Get_Count(void);

void update_time(RTC_Handle * GetTime);

int main()
{
		
	gpio_init();
	
	uart_init();
	
	rtc_init();	
	
	rtc_set_time(28,2,2020,12,00,53);
			
	static unsigned char count=0;
	
	while(1)
	{
			GetTime1=rtc_get_time();
		
			sprintf((char*)show,(const char*)" Date: %02d-%02d-%02d ",GetTime1->Day,GetTime1->Month,GetTime1->Year);	
			uart_tx_string(show);	
			sprintf((char*)show,(const char*)" Time: %02d-%02d-%02d\r\n",GetTime1->Hour,GetTime1->Minutes,GetTime1->Second);	
			uart_tx_string(show);		
			delay();	
		
		#if 1 //test GPIO
		
			if(GetTime1->Hour==12 && GetTime1->Minutes==1)
			{
				GPIOC->ODR |= (1<<13);				
			}
			else if(GetTime1->Hour==12 && GetTime1->Minutes==2)
			{
				GPIOC->ODR &=~ (1<<13);				
			}
		
		#endif			
	
	}
	
	return 0;
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
	
	USART3->BRR &=0x00000000;
	USART3->BRR |=(52<<4); //mantissa
	USART3->BRR |=(1<<0); //Fraction
	
	
	/* 115200 baud */
	//USART3->BRR |=(4<<4); //mantissa
	//USART3->BRR |=(5<<0); //Fraction
	
	//USART_STD_BAUD_9600;
	//USART_HW_FLOW_CTRL_NONE;
	//USART_MODE_ONLY_TX;
	//USART_STOPBITS_1;
	//USART_WORDLEN_8BITS;
	//USART_PARITY_DISABLE;
	
	USART3->CR1 |=(1<<3); //tx enable
	USART3->CR1 |=(1<<13); //uart enable
	
}


void Wait_Finished()
{
	while( (RTC->CRL & (1<<5)) == 0);//Check RTOFF: RTC operation OFF to write RTC Registers	
	
}

void Clear_Sync()
{
	Wait_Finished();
	
	RTC->CRL &= ~(1<<3); //clear sync RSF bit	
	
}

void Wait_Sync()
{
	Wait_Finished();
	
	while( (RTC->CRL & (1<<3)) == 0);  //Check RSF: Registers synchronized flag		
	
}

void Enter_Config()
{
	Wait_Finished();
	
	//PWR->CR |=(1<<8); // enable backup domain write protection.
	
	RTC->CRL |= (1<<4);//Enter CNF: Configuration flag
}

void Exit_Config()
{
		
	Wait_Finished();
	
	//PWR->CR &=~(1<<8); // disable backup domain write protection.
	
	RTC->CRL &= ~(1<<4);//Exit CNF: Configuration flag

}



void rtc_init()
{		
	//1.enable the power and backup interface clocks
	
	////RCC->APB1ENR&=0x00000000;  //if this used uart clk will disabled
	
	RCC->APB1ENR |=(1<<28);   //Power interface clock enable
	
	RCC->APB1ENR |=(1<<27);  //Backup interface clock enable 	
	
	
	//2.enable access to backup registers
	PWR->CR &=0x00000000;	
	PWR->CR |=(1<<8); // enable backup domain write protection. DBP
	
		
	//3.RTC clock source selection
	RCC->BDCR &=0x00000000;
	
	RCC->BDCR  |= (1<<8); //LSE oscillator clock used as RTC clock
	RCC->BDCR  |= (1<<0); //LSEON: External low-speed oscillator enable
	
	RCC->BDCR  |= (1<<15);//RTCEN: RTC clock enable	

	
	while( (RCC->BDCR  & (1<<1)) == 0);//Check LSERDY: External low-speed oscillator ready or not
	
	//4.RTC Config
	
	//RTC->CRL &=0x0000;

	Wait_Finished();
	
	Clear_Sync();
	
  Wait_Sync();
	

	/*5.RTC prescaler Config */	
	rtc_prescaler(0x7fff);		
		
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
		uart_tx(*data++);
	
	}
	
	while( !(USART3->SR & (1<<6)));//tx complete check;
}


void delay()
{
		static unsigned int i=0;
		for(i=0;i<1000;i++);
		for(i=0;i<5000;i++);
	
}


void rtc_set_time(unsigned int date,unsigned int month,unsigned int year,unsigned int hour,unsigned int min,unsigned int sec)
{
	SetTime.Day=date;
	SetTime.Month=month;
	SetTime.Year=year;
	
	SetTime.Hour=hour;
	SetTime.Minutes=min;
	SetTime.Second=sec;	
	
	//Generate Seconds Counts for Date and Time	
	Make_Time(&SetTime);
}

RTC_Handle* rtc_get_time()
{
	return Break_Time();
}


unsigned char check_leap(unsigned int year)
{
	
	if(((year%4==0) && (year%100 !=0 )) || (year%400==0 ))
	{
		
		return 1;	
		
	}	
	
	return 0;
}


void Make_Time()
{
		
	/*total hour */
	count_seconds+= ((unsigned long int) (SetTime.Hour *60*60));
	
	/*total minute */
	count_seconds+= ((unsigned long int)  (SetTime.Minutes *60));
	/* total seconds */
	count_seconds += ((unsigned long int)  SetTime.Second );
	
	/* Config date  */
		unsigned char date_config=1;
		
		if(date_config ==1 )
		{
			date_config=0;
			GetTime.Day=SetTime.Day;
			GetTime.Month=SetTime.Month;
			GetTime.Year=SetTime.Year;		
		}
			
		if(count_seconds>=0xffffffff)
		{
			count_seconds=0;
		}	
	
	//store total counts in RTC count registers	
	RTC_set_Count(count_seconds);
	
}

void update_time(RTC_Handle * GetTime)
{
	
	unsigned long int count_seconds1=0;
		/*total hour */
	count_seconds1+= ((unsigned long int) (GetTime->Hour *60*60));
	
	/*total minute */
	count_seconds1+= ((unsigned long int)  (GetTime->Minutes *60));
	/* total seconds */
	count_seconds1 += ((unsigned long int)  GetTime->Second );
	

		if(count_seconds1>=0xffffffff)
		{
			count_seconds1=0;
		}	
	
	//store total counts in RTC count registers	
	RTC_set_Count(count_seconds1);
	
}


RTC_Handle* Break_Time()
{

	
	/* Time splitting */	
	temp_days=(RTC_Get_Count());  //remaining hours taken from total days count
	
	if(temp_days>=0xffffffff)
	{
		temp_days=0;
		
	}	
	unsigned long int hour=0,day_elapsed=0,loop=0;
	volatile unsigned char update=0;
	
	hour= (temp_days/3600) ;  //split hour
	GetTime.Minutes= ((temp_days%3600)/60) ;  //min split
	GetTime.Second=  ((temp_days%3600)%60);  //sec split
	
	/* find day elapsed if one day completed*/
	if(hour>=24)
	{
		//day_elapsed= hour/24;
		
		GetTime.Hour=hour%24;
		
		if(GetTime.Hour%24 == 0)
		{
			update=1;hour=0;
			update_time(&GetTime);
		}
		else
		{
			update=0;
		}

	}		
	else
	{
		GetTime.Hour=hour;
	}
	
	
	unsigned long int month=0,year=0,day=0;
	
	day=GetTime.Day;
	month=GetTime.Month;
	year=GetTime.Year;
	

	
if(update==1) 
{
	
	  update=0;
	
		if( (month==1) || (month==3) ||(month==5) ||(month==7) /
			(month==8) ||(month==10) || (month==12) ) 
		{
			if(day<31)
			{
				day++;
			}
			else
			{				
				if(month != 12 )
				{
					month++;
					day=1;
				}
				else
				{					
					month=1;
					day=1;
					year++;
				}				
			
			}
		}
		
		else if( (month==4) || (month==6) ||(month==9) ||(month==11) )
		{
			if(day<30)
			{
				day++;
			}
			else{
				
					month++;
					day=1;
				}	
			
		}
		
		else if(month==2) 
		{
			if(day<28)
			{
				day++;
				
			}
			else if(day==28)
			{
				if(check_leap(year))
				{
					day++;
				}
				else
				{
					month++;
					day=1;
				}				
			}
			else if(day ==29)
			{
				month++;
				day=1;
			}			
		}

}
  GetTime.Day=day;
	GetTime.Month=month;
	GetTime.Year=year;
	
	return (RTC_Handle*)&GetTime;
	
	
}

void rtc_prescaler(unsigned int pre)
{
		 Wait_Finished();
		 Clear_Sync();
		 Wait_Sync();
		 Enter_Config();	

		 RTC->PRLH = (pre >> 16) & 0xffff;	
		 RTC->PRLL = pre & 0xffff;	

		 Exit_Config();
		 Wait_Finished();	

}


void RTC_set_Count(unsigned long int count_seconds)
{
		PWR->CR |= (1<<8); // enable backup domain write protection. DBP
	
		Wait_Finished();
		Clear_Sync();
		Wait_Sync();

		Enter_Config();
		RTC->CNTH = (count_seconds>>16) & 0xFFFF;
		RTC->CNTL = (count_seconds) & 0xFFFF;

		Exit_Config();
		Wait_Finished();	
	
		PWR->CR &= ~(1<<8); // disable backup domain write protection. DBP
	
}

unsigned long int  RTC_Get_Count(void)
{

	unsigned long int data_h,data_l;

	Wait_Finished();
	Clear_Sync();
  Wait_Sync();
	
	do{
		
		data_h=RTC->CNTH;
		data_l=RTC->CNTL;
		
	}while(data_h != RTC->CNTH );
	
	return ((data_h<<16)|data_l);

}




