/** @file main.c
*
* @brief
* @author Aaron Vorse
* @date   4/29/2022
* @contact aaron.vorse@embeddedresume.com
*
*/

#include "main.h"
#define DUMMY_BYTE 0x77


/*
****************************************************
********** Private Function Prototypes *************
****************************************************
*/
void main_peripherals_init(void);
void test_function(void);


/*
****************************************************
**************** Global Variables ******************
****************************************************
*/
uint32_t global_initialized = 5;
uint32_t global_uninitialized;


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

   /* main system loop */
   while(1)
   {
      ConsoleProcess();
      states_update_main_event();
      states_update_main_state();
      states_update_led();
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
   //uart1_init(BR_PRESCALER_115200); //Set UART1 to 115200 baud rate. For alternatives see uart.h
   uart1_init(BR_PRESCALER_9600); //Set UART1 to 115200 baud rate. For alternatives see uart.h

   //Clears all startup noise from the uart bus and jump to a new line
   uart1_send_byte(DUMMY_BYTE);
   uart1_printf("\r\n");

   button_mode_init();
   button_auto_init();
   button_led_init();
   button_manual_init();

   ConsoleInit();

   timers_timer5_init();
   timers_timer11_init();
   magnet_init();
}


/*** end of file ***/

