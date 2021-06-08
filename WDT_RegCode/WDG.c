

#include "stm32f10x.h"                  // Device header

#include "WDG.h"

#include<stdint.h>


	/********************************************************************** 
			Window Watchdog Timeout calculation:
			
			APB1 clk =30mhz if sysclk is 60mhz

			counter_val = (time(ms) * 30000000 / 4096*4*1000) +64

			window_val = counter_val - (time(ms) * 30000000 / 4096*4*1000)

	**********************************************************************/

/*This Function used to init WDT */

void WDG_Init(void)
{
	/* Clock Setup */
	RCC->APB1ENR  |= (1<<11);  //WWDT clk enable

	/* WWDT Setup */
	WWDG->CR &=0x00000000; //reset

	WWDG->CR =  (109<<0); //max timout 50ms  @Ref: counter_val:109

	WWDG->CFR &=0x00000000; //reset

	WWDG->CFR = (109<<0);  //min timout 1ms  @Ref: window_val:109

	WWDG->CFR |= (3<<7) ; //pre 8

	WWDG->CFR &= ~(1<<9) ; //Early wakeup interrupt -EWI disable

	WWDG->CR |= (1<<7)  ;  //Enable WDT

}

/*This function used to trigger within 1ms to 50ms,otherwise WDT reset microcontroller */

void WDG_Sync(void)
{

	WWDG->CR |=  (109<<0); //max timout 50ms

}

