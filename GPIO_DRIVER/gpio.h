#ifndef __GPIO_DRIVER_H__
#define __GPIO_DRIVER_H__


/******************************************************************************************************************************
 *												      Preprocessor Constants
 ******************************************************************************************************************************/

#define PORTA_CLOCK_ENABLE()   RCC->APB2ENR |= (1<<2)
#define PORTB_CLOCK_ENABLE()   RCC->APB2ENR |= (1<<3)
#define PORTC_CLOCK_ENABLE()   RCC->APB2ENR |= (1<<4)
#define PORTD_CLOCK_ENABLE()   RCC->APB2ENR |= (1<<5)

#define IsClockEnabled_PortA() (RCC->APB2ENR & (1<<2))
#define IsClockEnabled_PortB() (RCC->APB2ENR & (1<<3))
#define IsClockEnabled_PortC() (RCC->APB2ENR & (1<<4))
#define IsClockEnabled_PortD() (RCC->APB2ENR & (1<<5))


/******************************************************************************************************************************
 *												      Typedef Declarations
 ******************************************************************************************************************************/

/* 
 * Enumeration to represent the PORTS
 */
typedef enum _Port_EN
{
    PA,
    PB,
    PC,
    PD
}Port_EN;

/* 
 * Enumeration to represent the PORTS pins
 */
typedef enum _Pin_EN
{
    P0,P1,P2,P3,P4,P5,P6,P7,P8,P9,P10,P11,P12,P13,P14,P15
}Pin_EN;


/* 
 * Enumeration to represent the Mode of GPIO pins.
 */
typedef enum _Config_PinMode_EN
{
    Input, //Input Mode.
    Speed_10MHz_Output,
    Speed_2MHz_Output,
    Speed_50MHz_Output,
}Config_PinMode_EN;

/* 
 * Enumeration to represent pin state.
 */
typedef enum _PinState_EN
{
    High,
    Low
}PinState_EN;


/* 
 * Enumeration to represent various states of the pin.
 */
typedef enum _Config_PinState_EN
{
    Input_Analog,
    Input_Floating,
    Input_Up_Down_Pull,
    Reserverd,
    Output_PushPull=0,
    Output_OpenDrain,
    Output_Alternate_PushPull,
    Output_Alternate_Opendrain
}Config_PinState_EN;

/* 
 * Structure definition for holding configuration values during initialization of a pin.
 */
typedef struct _GPIO_Config
{
    Port_EN CurrentPort;
    Pin_EN CurrentPin;
    Config_PinMode_EN PinMode;
    Config_PinState_EN PinState;
}GPIO_Config;


/******************************************************************************************************************************
 *												      Fucntion Declarations
 ******************************************************************************************************************************/

/*
 * @brief  : To Configure a pin mode
 * @para   : GPIO_Config (structure) which holds the value of port,pin,pin state and mode to be configured.
 * @return : void.
 */

void GPIO_Config_Pin(GPIO_Config *ConfigPort);

/*
 * @brief  : To write data to gpio pin. This function will write a pin only if it is configured in output mode.
 * @para   : Port of current pin, current pin number, state to be written.
 * @return : void.
 */

void GPIO_Write_Data(Port_EN CurrentPort,Pin_EN CurrentPin,unsigned char State);

/*
 * @brief  : To read data from a pin. This function will read a pin only if it is configured in input mode.
 * @para   : Port of current pin, current pin number
 * @return : State of the pin (0,1)
 */

unsigned char GPIO_Read_Data(Port_EN CurrentPort,Pin_EN CurrentPin);

/*
 * @brief  : To toggle a pin in a port, This function toggles a pin only if it configured in output mode.
 * @para   : Port of Current pin, current pin number.
 * @return : void.
 */

void GPIO_Toggle_Pin(Port_EN CurrentPort, Pin_EN CurrentPin);


#endif //__GPIO_DRIVER_H__
