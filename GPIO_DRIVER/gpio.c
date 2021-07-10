#include<stm32f10x.h>
#include <stdint.h>
#include "gpio.h"

/******************************************************************************************************************************
 *												      Variable Definitions
 ******************************************************************************************************************************/

/*
 *  @brief : uint32_t arrays to point the memory locations of GPIO Configuration registers. for each pin there will be
 *           4 bit (4 * 16 = 64 bits)
 */
uint32_t volatile *const GPIO_Con_Low[4] = {&GPIOA->CRL, &GPIOB->CRL, &GPIOC->CRL, &GPIOD->CRL};
uint32_t volatile *const GPIO_Con_High[4] = {&GPIOA->CRH, &GPIOB->CRH, &GPIOC->CRH, &GPIOD->CRH};


/*
 *  @brief : uint32_t arrays to point the memory locations of GPIO Data registers. which can be used to read/write
 *           data to/from the GPIO pins.
 */
uint32_t volatile *const GPIO_Input_Data[4] = {&GPIOA->IDR, &GPIOB->IDR, &GPIOC->IDR, &GPIOD->IDR};
uint32_t volatile *const GPIO_Output_Data[4] = {&GPIOA->ODR, &GPIOB->ODR, &GPIOC->ODR, &GPIOD->ODR};


/******************************************************************************************************************************
 *												      Function Definitions
 ******************************************************************************************************************************/


void GPIO_Config_Pin(GPIO_Config * ConfigPort )
{
    uint32_t volatile *Control_Reg;

    if (ConfigPort->CurrentPin < P8)
    {
        Control_Reg = GPIO_Con_Low[ConfigPort->CurrentPort];
    }

    else
    {
        Control_Reg = GPIO_Con_High[ConfigPort->CurrentPort];
    }

    *Control_Reg &= (~((uint32_t)0x3 << (4 * (ConfigPort->CurrentPin % 8)))); //Clear the bits to be set.
    *Control_Reg |= (ConfigPort->PinMode << (4 * (ConfigPort->CurrentPin % 8)));

    *Control_Reg &= (~((uint32_t)0x3 << ((4 * (ConfigPort->CurrentPin % 8)) + 2))); //Clear the bits to be set.
    *Control_Reg |= (ConfigPort->PinState << ((4 * (ConfigPort->CurrentPin % 8)) + 2));
}

void GPIO_Write_Data(Port_EN CurrentPort, Pin_EN CurrentPin,unsigned char State)
{
    if (State == High )
    {
        *GPIO_Output_Data[CurrentPort] |=  (1 << CurrentPin);
    }

    else
    {
        *GPIO_Output_Data[CurrentPort] &= (~(1 << CurrentPin));
    }
}

unsigned char GPIO_Read_Data(Port_EN CurrentPort, Pin_EN CurrentPin)
{
    unsigned char CurrentState = 0;

    uint32_t volatile *Control_Reg;

    if (CurrentPin < P8)
    {
        Control_Reg = GPIO_Con_Low[CurrentPort];
    }

    else
    {
        Control_Reg = GPIO_Con_High[CurrentPort];
    }

    /*We will read pin state only if it is configured in input mode. */
    if (((*Control_Reg >> (4 * (CurrentPin % 8))) & 0x3) == 0)
    {
        CurrentState = (*GPIO_Input_Data[CurrentPort] >> CurrentPin) & 0x00000001;
    }

    return CurrentState;
}

void GPIO_Toggle_Pin(Port_EN CurrentPort, Pin_EN CurrentPin)
{
    uint32_t volatile *Control_Reg;

    if (CurrentPin < P8)
    {
        Control_Reg = GPIO_Con_Low[CurrentPort];
    }

    else
    {
        Control_Reg = GPIO_Con_High[CurrentPort];
    }

    /*We will toggle pin state only if it is configured in output mode. */
    if (((*Control_Reg >> (4 * (CurrentPin % 8))) & 0x3) != 0)
    {
        *GPIO_Output_Data[CurrentPort] ^= ((uint32_t)1 << CurrentPin);
    }
}
