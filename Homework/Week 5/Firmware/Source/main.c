/** @file main.c
*
* @brief
* @author Aaron Vorse
* @date   4/29/2022
* @contact aaron.vorse@embeddedresume.com
*
*/

#include "main.h"


/*
****************************************************
********** Private Function Prototypes *************
****************************************************
*/
void main_peripherals_init(void);



/*!
* @brief This is the main system loop. It initializes each peripheral module then jumps into an infinite loop.
*        This loop constantly services the system's various state machines as the user interacts with the device.
* @param[in] NONE
* @return NONE
*
*/
int
main(void)
{
   //Initialize system and peripheral clocks
   system_clock_init();

   //Initialize MCU peripherals and seed background state machines
   main_peripherals_init();

   ConsoleInit();

   /* main system loop */
   while(1)
   {
   	ConsoleProcess();
   }

   return(0);
}


/*!
* @brief Initializes the MCU's peripherals and system settings and seeds various system state machines
* @param[in] NONE
* @return NONE
*
*/
void
main_peripherals_init(void)
{
   //Init UART for the UART to USB for serial communication
   uart1_init(BR_PRESCALER_115200); //Set UART1 to 115200 baud rate. For alternatives see uart.h

   gpio_gen_output_init(LED_PIN);
}

/*** end of file ***/
