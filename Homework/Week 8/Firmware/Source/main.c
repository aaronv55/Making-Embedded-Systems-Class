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

   //Clears all startup noise from the uart bus and jump to a new line
	uart1_send_byte(DUMMY_BYTE);
	uart1_printf("\r\n");


   char tmp_str[60]; //Buffer to hold converted addresses as strings

   //Convert address of initialized & uninitialized global variables to strings and send over uart
   sprintf(tmp_str,"Initialized Global:\n\r  0x%X \n\r", &global_initialized);
	uart1_printf(tmp_str);

   sprintf(tmp_str,"Uninitialized Global:\n\r  0x%X \n\r", &global_uninitialized);
	uart1_printf(tmp_str);

	//Dummy function to read addresses of local variables
   test_function();

   //Find heap pointer address
   uint32_t test_buffer_before = (uint32_t*) malloc(10);
   sprintf(tmp_str,"Initial Heap Pointer:\n\r  0x%X \n\r", test_buffer_before);
	uart1_printf(tmp_str);

	uint32_t test_buffer_after = (uint32_t*) malloc(10);
   sprintf(tmp_str,"Heap Pointer after 10 uint32s:\n\r  0x%X \n\r", test_buffer_after);
	uart1_printf(tmp_str);

   /* main system loop */
   while(1)
   {

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
}


/*!
* @brief Used as an example function to find various variable/register addresses
* @param[in] NONE
* @return var_in_func_normal
*
*/
void
test_function(void)
{
   uint32_t stack_value = __get_MSP();
   uint32_t *stack_value_confirmation = &stack_value;

   char tmp_str[50];

   //Stack pointer address testing
   sprintf(tmp_str,"Stack Pointer:\n\r  0x%X \n\r", stack_value);
	uart1_printf(tmp_str);
   sprintf(tmp_str,"Stack Pointer Confirmation:\n\r  0x%X \n\r", stack_value_confirmation);
	uart1_printf(tmp_str);

	//Local variable address testing
	static uint8_t var_func_static = 0;
   uint8_t var_func_normal = 0;

   sprintf(tmp_str,"Static variable in Function:\n\r  0x%X \n\r", &var_func_static);
	uart1_printf(tmp_str);

	sprintf(tmp_str,"Variable in Function:\n\r  0x%X \n\r", &var_func_normal);
	uart1_printf(tmp_str);
}


/*** end of file ***/

