/** @file buttons.c
*
* @brief  This file contains drivers for physical user buttons
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
*
*/

#include "buttons.h"

/*
****************************************************
************* File-Static Variables ****************
****************************************************
*/
static uint8_t buttons_mode_detection = 0;
static uint8_t buttons_auto_detection = 0;
static uint8_t buttons_led_detection = 0;

/*
****************************************************
********** Private Function Prototypes *************
****************************************************
*/


/*
****************************************************
********** Public Function Definitions *************
****************************************************
*/


/*
****************************************************
********** Private Function Definitions *************
****************************************************
*/

/**
* @brief Initialize external button as an interrupt pin
* @param[in] NONE
* @return NONE
*
*/
void
button_mode_init(void)
{
   //Initialize pin to input, no pull-up/down, low speed
   gpio_gen_input_init(BUTTON_MAIN_MODE);

   //Unmask interrupt for pin 0
   EXTI -> IMR |= EXTI_IMR_IM0;

   //Set pin as an interrupt
   SYSCFG -> EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PC; //Tie port C to interrupt line 0
   EXTI->RTSR |= EXTI_RTSR_TR0; // Set trigger pin 0 to rising edge, it has an external pull down
   NVIC_SetPriority(EXTI0_IRQn,2); // Set interrupt priority to 2
   NVIC_EnableIRQ(EXTI0_IRQn);
}


/**
* @brief Initialize external button as an interrupt pin
* @param[in] NONE
* @return NONE
*
*/
void
button_auto_init(void)
{
   //Initialize pin to input, no pull-up/down, low speed
   gpio_gen_input_init(BUTTON_AUTO_MODE);

   //Unmask interrupt for pin 1
   EXTI -> IMR |= EXTI_IMR_IM1;

   //Set pin as an interrupt
   SYSCFG -> EXTICR[0] |= SYSCFG_EXTICR1_EXTI1_PC; //Tie port C to interrupt line 1
   EXTI->RTSR |= EXTI_RTSR_TR1; // Set trigger pin 1 to rising edge, it has an external pull down
   NVIC_SetPriority(EXTI1_IRQn,2); // Set interrupt priority to 2
   NVIC_EnableIRQ(EXTI1_IRQn);
}


/**
* @brief Initialize external button as an interrupt pin
* @param[in] NONE
* @return NONE
*
*/
void
button_led_init(void)
{
   //Initialize pin to input, no pull-up/down, low speed
   gpio_gen_input_init(BUTTON_LED);

   //Unmask interrupt for pin 2
   EXTI -> IMR |= EXTI_IMR_IM2;

   //Set pin as an interrupt
   SYSCFG -> EXTICR[0] |= SYSCFG_EXTICR1_EXTI2_PC; //Tie port C to interrupt line 2
   EXTI->RTSR |= EXTI_RTSR_TR2; // Set trigger pin 2 to rising edge, it has an external pull down
   NVIC_SetPriority(EXTI2_IRQn,2); // Set interrupt priority to 2
   NVIC_EnableIRQ(EXTI2_IRQn);
}


/**
* @brief Initialize external button as input
* @param[in] NONE
* @return NONE
*
*/
void
button_manual_init(void)
{
   //Initialize pin to input, no pull-up/down, low speed
   gpio_gen_input_init(BUTTON_MANUAL);
}

/**
* @brief Check if manual button is currently being pressed
* @param[in] NONE
* @return NONE
*
*/
uint8_t
button_manual_status(void)
{
   uint8_t button_press = gpio_read_pin(BUTTON_MANUAL);

   return(button_press);
}


/**
* @brief Check if mode button has been pressed
* @param[in] NONE
* @return NONE
*
*/
uint8_t
button_mode_status(void)
{
   return(buttons_mode_detection);
}


/**
* @brief Reset the button flag so it can be used again
* @param[in] NONE
* @return NONE
*
*/
void
button_mode_reset(void)
{
	buttons_mode_detection = 0;
}

/**
* @brief Check if auto button has been pressed
* @param[in] NONE
* @return NONE
*
*/
uint8_t
button_auto_status(void)
{
   return(buttons_auto_detection);
}


/**
* @brief Reset the button flag so it can be used again
* @param[in] NONE
* @return NONE
*
*/
void
button_auto_reset(void)
{
	buttons_auto_detection = 0;
}



/**
* @brief Check if led button has been pressed
* @param[in] NONE
* @return NONE
*
*/
uint8_t
button_led_status(void)
{
   return(buttons_led_detection);
}

/**
* @brief Reset the button flag so it can be used again
* @param[in] NONE
* @return NONE
*
*/
void
button_led_reset(void)
{
	buttons_led_detection = 0;
}


/**
* @brief ISR that is called when PC0 experiences a
*        rising edge (main mode button is pressed)
* @param[in] NONE
* @return  NONE
*/
void
EXTI0_IRQHandler(void)
{
   buttons_mode_detection = 1;

   //Clear interrupt flag
   EXTI->PR = EXTI_PR_PR0;
}

/**
* @brief ISR that is called when PC1 experiences a
*        rising edge (automatic mode button is pressed)
* @param[in] NONE
* @return  NONE
*/
void
EXTI1_IRQHandler(void)
{
   buttons_auto_detection = 1;

   //Clear interrupt flag
   EXTI->PR = EXTI_PR_PR1;
}

/**
* @brief ISR that is called when PC2 experiences a
*        rising edge (led button is pressed)
* @param[in] NONE
* @return  NONE
*/
void
EXTI2_IRQHandler(void)
{
   buttons_led_detection = 1;

   //Clear interrupt flag
   EXTI->PR = EXTI_PR_PR2;
}


/*** end of file ***/
