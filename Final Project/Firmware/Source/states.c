/** @file states.c
*
* @brief This file contains the main logic for the system's state machine.
*        It defines the various menu attributes and how the system transitions
*        from one state to another.
* @author Aaron Vorse
* @date   6/1/2022
* @contact aaron.vorse@embeddedresume.com
*
* @LICENSE:
*
*  Licensed to the Apache Software Foundation (ASF) under one
*  or more contributor license agreements.  See the NOTICE file
*  distributed with this work for additional information
*  regarding copyright ownership.  The ASF licenses this file
*  to you under the Apache License, Version 2.0 (the
*  "License"); you may not use this file except in compliance
*  with the License.  You may obtain a copy of the License at
*
*    http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing,
*  software distributed under the License is distributed on an
*  "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
*  KIND, either express or implied.  See the License for the
*  specific language governing permissions and limitations
*  under the License.
*/

#include "states.h"



/*
****************************************************
***** Private Types and Structure Definitions ******
****************************************************
*/

typedef enum e_state_main_tag
{
   state_idle,
   state_auto_pulse,
   state_manual,
   max_main_state

} e_state_main;


typedef enum e_event_main_tag
{
   no_event,
   event_button_mode,
   max_main_event

} e_event_main;


/*
****************************************************
************* File-Static Variables ****************
****************************************************
*/
static e_event_main current_event = no_event;
static e_state_main current_state = state_idle;
static uint8_t current_mag_algo = 0;


/*
****************************************************
********** Private Function Prototypes *************
****************************************************
*/
void states_main_idle(void);
void states_main_auto_pulse(void);
void states_main_manual(void);
void states_mag_service(void);
void states_mag_algo_select(void);
/*
****************************************************
********** Public Function Definitions *************
****************************************************
*/


/*!
* @brief This looks at all possible events in the main system state machine, ranks them in order
*        of importance, then sets the current main event to the most important.
* @param[in] NONE
* @return NONE
*
*/
void
states_update_main_event(void)
{
   //Leaving this function for future flexibility
   if(button_mode_status())
   {
	   current_event = event_button_mode;
	   button_mode_reset();
   }
}


/*!
* @brief Uses the current main event along with the current main state to determine the next appropriate state
* @param[in] NONE
* @return NONE
*
*/
void
states_update_main_state(void)
{
	//Update next state depending on current state and current event
	if(event_button_mode == current_event)
	{
		current_event = no_event; //Clear event for next pass

		//Determine next appropriate state
		switch(current_state)
		{
		   case state_idle:
			   current_state = state_auto_pulse;
			   break;
		   case state_auto_pulse:
			   current_state = state_manual;
			   break;
		   case state_manual:
			   current_state = state_idle;
			   break;
		   default:
			   break;

		}
	}

   //Call the appropriate function based on the current state
   switch(current_state)
   {
      case state_idle:
	     states_main_idle();
	     break;
	  case state_auto_pulse:
		  states_main_auto_pulse();
		  break;
      case state_manual:
         states_main_manual();
         break;
      default:
         break;
   }
}


/*!
* @brief This updates the brightness of the LEDs based on user input
* @param[in] NONE
* @return NONE
*
*/
void
states_update_led(void)
{

   //Array to cycle through
   const uint16_t brightness_options[4] = {LED_MAG_OFF, LED_MAG_LOW, LED_MAG_MED, LED_MAG_HIGH};
   static uint8_t current_option = 0;

   //Only toggle brightness if button was pushed
   if(button_led_status())
   {

	   button_led_reset();

	   //Increment brightness from options list
	   if(3 == current_option)
	   {
	      current_option = 0;
	   }

	   else
	   {
	      current_option++;
	   }

	   //Push new brightness to LED
	   led_set_mag(brightness_options[current_option]);
   }

}

/*!
* @brief Print the current state to the serial terminal
* @param[in] NONE
* @return NONE
*
*/
void
states_print_state(void)
{
   const char states_list[3][20] = {"\r\n Idle State\r\n\0", "\r\n Auto State\r\n\0", "\r\n Manual State\r\n\0"};

   uart1_printf(states_list[current_state]);

}


/*
****************************************************
********** Private Function Definitions *************
****************************************************
*/


/*!
* @brief In this state, the system will turn off all external peripherals and wait for a button press
* @param[in] NONE
* @return NONE
*
*/
void
states_main_idle(void)
{
	magnet_set_mag(MAGNET_MAG_OFF);
}


/*!
* @brief In this state, the system will go through one of its predefined electromagnet cycles
* @param[in] NONE
* @return NONE
*
*/
void
states_main_auto_pulse(void)
{
   states_mag_service();
}

/*!
* @brief In this state, the system will pulse the electromagnet based on the user's button presses
* @param[in] NONE
* @return NONE
*
*/
void
states_main_manual(void)
{
   if(button_manual_status())
   {
      magnet_set_mag(MAGNET_MAG_MAX);
   }

   else
   {
	   magnet_set_mag(MAGNET_MAG_OFF);
   }

	//uart1_printf("\n\r State: Manual\n\r");
}


/*!
* @brief This will updated the next electromagnet magnitude based on a selectable algorithm
* @param[in] NONE
* @return NONE
*
*/
void
states_mag_service(void)
{

   static uint8_t i = 0;
   static uint8_t previous_mag_algo = 0;

   states_mag_algo_select();

   //If the selected magnet algorithm has changed, reset i to start from the beginning
   if(current_mag_algo != previous_mag_algo)
   {
      i = 0;
   }

   const uint16_t magnet_lookup[STATES_MAGNETALGO_MAX][40] =
   {
         {0, 544, 1088, 1632, 2176, 2720, 3264, 3808, STATES_MAGNETALGO_STOP},  //Very short ramp up

         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 272, 544, 816, 1088, 1360,1632,
          1904, 2176, 2448, 2720, 2992, 3264, 3536, 3808, STATES_MAGNETALGO_STOP}, //Medium-length ramp up

         {0, 136, 272,408,544,680,816,952,1088,1224, 1360,1496,1632,1768,
          1904, 2040, 2176, 2312, 2448, 2584, 2720, 2856, 2992, 3128, 3264, 3400,  //Long ramp up
          3536, 3672, 3808, 3944, STATES_MAGNETALGO_STOP},
   };

   //If timer5 has expired, update the electromagnet magnitude to the next value
   if(timers_tim5_get_status())
   {

      timers_tim5_reset_flag();

      //Check if the current algorithm has completed
      if(magnet_lookup[current_mag_algo][i] != STATES_MAGNETALGO_STOP)
      {
         i++;
         magnet_set_mag(magnet_lookup[current_mag_algo][i]); //Push next value to external electromagnet
      }

      else
      {
         i = 0;
      }

   }

   previous_mag_algo = current_mag_algo;

}


/*!
* @brief Checks if the user has pressed the auto button and updates the electromagnet algorithm accordingly
* @param[in] NONE
* @return NONE
*
*/
void
states_mag_algo_select(void)
{
   //Check if user has pressed "auto" button
   if(button_auto_status())
   {
      button_auto_reset();

      //Check if we're at the end of our list of different algorithms. If not, increment. If so, reset.
      if((STATES_MAGNETALGO_MAX - 1) > current_mag_algo)
      {
         current_mag_algo++;
      }

      else
      {
         current_mag_algo = 0;
      }
   }

}

/* end of file */
