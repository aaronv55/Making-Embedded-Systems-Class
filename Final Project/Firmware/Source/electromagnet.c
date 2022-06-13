/** @file electromagnet.c
*
* @brief  This file contains drivers for playing audio files via the DAC
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

#define MAGNET_TERMINATION_CHAR 5000; //Used to signal the end of an electromagnet pattern

#include "electromagnet.h"

/*
****************************************************
************* File-Static Variables ****************
****************************************************
*/

static const uint16_t magnet_fast_pattern[] =
{

};

/*
****************************************************
********** Private Function Prototypes *************
****************************************************
*/
void dac_dma1_init(void);


/*
****************************************************
********** Public Function Definitions *************
****************************************************
*/


/*!
* @brief Initializes the DAC
* @param[in] NONE
* @return NONE
*/
void
magnet_init(void)
{
   //Initialize peripheral clocks
   RCC->APB1ENR |= RCC_APB1ENR_DACEB;
   RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

   //Set up basic GPIO settings
   gpio_func_init(GPIOA, 5, GPIO_MODER_ALTERNATE_FUNCTION);
   gpio_speed_init(GPIOA, 5, GPIO_OSPEEDR_VERY_HIGH);
   gpio_type_init(GPIOA, 5, GPIO_OTYPER_PUSH_PULL);
   gpio_pupd_init(GPIOA, 5, GPIO_PUPDR_NONE);
   GPIOA->AFR[0] &= ~(GPIO_AFRL_PIN5_MASK);

   //Enable DAC1
   dac_enable();

   //Set initial voltage to 0v
   DAC1->DHR12R1 |= 0x0000;
}


/*!
* @brief Control the magnitude of current through the electromagnet
* @param[in] NONE
* @return NONE
*/
void
magnet_set_mag(uint16_t tmp_magnitude)
{
   //Bounds check for 12 bit, right-aligned DAC register
   if(4095 > tmp_magnitude)
   {
      DAC1->DHR12R1 = tmp_magnitude;
   }

}




/*** end of file ***/

