/** @file base_gpio_drivers.c
*
* @brief  Drivers for GPIO initialization, and basic pin manipulation. Pin registers
* are written to directly.
* @note The reference manual for the stm32F410RBT6 can be found at https://www.st.com
*
* @author Aaron Vorse
* @date   3/14/2020
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

#include "base_gpio_drivers.h"
#include <stdint.h>

/*
****************************************************
************ Static Global Variables ***************
****************************************************
*/


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

/*!
* @brief Enables the peripheral clock for the corresponding port.
* @param[in] p_gpio_tmp Pointer to GPIO structure.
* @param[in] pin_number Pin number of desired port
* @return  NONE
*/
void
gpio_clk_init(GPIO_TypeDef * p_gpio_tmp, uint8_t pin_number)
{
   if (p_gpio_tmp == GPIOA)
   {
      RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
   }

   else if (p_gpio_tmp == GPIOB)
   {
      RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
   }

   else if (p_gpio_tmp == GPIOC)
   {
      RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
   }

}

/*!
* @brief Initializes the desired pin function
* @param[in] p_gpio_tmp Pointer to GPIO structure.
* @param[in] pin_number Pin number of desired port
* @param[in] pin_function General mode of gpio pin
* @return NONE
* @warning Assumes the appropriate peripheral clock has already been set.
* @note Options are GPIO_MODER_INPUT, GPIO_MODER_GENERAL_OUTPUT,
* GPIO_MODER_ALTERNATE_FUNCTION, GPIO_MODER_ANALOG
*/
void
gpio_func_init(GPIO_TypeDef * p_gpio_tmp, uint8_t pin_number, uint8_t pin_func)
{

   if (!(GPIO_MODER_ANALOG < pin_func))  //only modes of 0x00 to 0x03 accepted
   {
      p_gpio_tmp->MODER &= ~(0x3ul << (2*pin_number));
      p_gpio_tmp->MODER |= (pin_func << (2*pin_number));
   }

}

/*!
* @brief Initializes the desired output type.
* @param[in] p_gpio_tmp Pointer to GPIO structure.
* @param[in] pin_number Pin number of desired port.
* @param[in] pin_type Output mode of pin, either open drai or push-pull.
* @return NONE
* @warning Assumes the appropriate peripheral clock has already been set.
* @note Options are GPIO_OTYPER_PUSH_PULL, GPIO_OTYPER_OPEN_DRAIN
*/
void
gpio_type_init(GPIO_TypeDef * p_gpio_tmp, uint8_t pin_number, uint8_t pin_type)
{

   if (!(GPIO_OTYPER_OPEN_DRAIN < pin_type))  //only modes of 0x00 to 0x03 accepted
   {
      p_gpio_tmp->OTYPER &= ~ (1ul << pin_number);
      p_gpio_tmp->OTYPER |= (pin_type << pin_number);
   }

}


/*!
* @brief Initializes the desired gpio speed
* @param[in] p_gpio_tmp Pointer to GPIO structure.
* @param[in] pin_number Pin number of desired port
* @param[in] pin_speed Speed of gpio
* @return NONE
* @warning Assumes the appropriate peripheral clock has already been set.
* @note Options are GPIO_OSPEEDR_LOW, GPIO_OSPEEDR_MEDIUM,
* GPIO_OSPEEDR_HIGH, GPIO_OSPEEDR_VERY_HIGH
*/
void
gpio_speed_init(GPIO_TypeDef * p_gpio_tmp, uint8_t pin_number, uint8_t pin_speed)
{

   if (!(GPIO_OSPEEDR_VERY_HIGH < pin_speed))  //only modes of 0x00 to 0x03 accepted
   {
      p_gpio_tmp->OSPEEDR &= ~(3ul << (2*pin_number));
      p_gpio_tmp->OSPEEDR |= (pin_speed << (2*pin_number));
   }

}


/*!
* @brief Initializes the desired gpio to pull up or pull down resistors
* @param[in] p_gpio_tmp Pointer to GPIO structure.
* @param[in] pin_number Pin number of desired port.
* @param[in] pin_pupd Pull-up pull-down select.
* @return NONE
* @warning Assumes the appropriate peripheral clock has already been set.
* @note "pupd" is pull-up pull-down. Options are GPIO_PUPDR_NONE, GPIO_PUPDR_PULL_UP,
* GPIO_PUPDR_PULL_DOWN
*/
void
gpio_pupd_init(GPIO_TypeDef * p_gpio_tmp, uint8_t pin_number, uint8_t pin_pupd)
{

	if (!(GPIO_PUPDR_PULL_DOWN < pin_pupd))  //only modes of 0x00 to 0x02 accepted
	{
	   p_gpio_tmp->PUPDR &= ~(3ul << (2*pin_number));
	   p_gpio_tmp->PUPDR |= (pin_pupd << (2*pin_number));
	}

}


/*!
* @brief Sets the desired pin to logic high, +3.3v.
* @param[in] p_gpio_tmp Pointer to GPIO structure.
* @param[in] pin_number Pin number of desired port.
* @return  NONE
* @warning Assumes the pin has alread been properly initialized.
*/
void
gpio_set(GPIO_TypeDef * p_gpio_tmp, uint8_t pin_number)
{
   p_gpio_tmp->BSRR = (1ul << pin_number);
}


/*!
* @brief Sets the desired pin to logic low, 0v.
* @param[in] p_gpio_tmp Pointer to GPIO structure.
* @param[in] pin_number Pin number of desired port.
* @return  NONE
* @warning Assumes the pin has already been properly initialized.
*/
void
gpio_clear(GPIO_TypeDef * p_gpio_tmp, uint8_t pin_number)
{
   //Writing "1" to the most significant two bytes clears the corresponding pin of the 16-bit port
   p_gpio_tmp->BSRR = (1ul << (pin_number + 16));
}


/*!
* @brief Initializes the desired pin to output mode with generic settings.
* @param[in] p_gpio_tmp Pointer to GPIO structure.
* @param[in] pin_number Pin number of desired port.
* @return  NONE
*/
void
gpio_gen_output_init(GPIO_TypeDef * p_gpio_tmp, uint8_t pin_number)
{
   gpio_clk_init(p_gpio_tmp, pin_number);
   gpio_func_init(p_gpio_tmp, pin_number, GPIO_MODER_GENERAL_OUTPUT);
   gpio_type_init(p_gpio_tmp, pin_number, GPIO_OTYPER_PUSH_PULL);
   gpio_speed_init(p_gpio_tmp, pin_number, GPIO_OSPEEDR_VERY_HIGH);
   gpio_pupd_init(p_gpio_tmp, pin_number, GPIO_PUPDR_NONE);
}


/*!
* @brief Initializes the desired pin to input mode with generic settings.
* @param[in] p_gpio_tmp Pointer to GPIO structure.
* @param[in] pin_number Pin number of desired port.
* @return  NONE
*/
void
gpio_gen_input_init(GPIO_TypeDef * p_gpio_tmp, uint8_t pin_number)
{
   gpio_clk_init(p_gpio_tmp, pin_number);
   gpio_func_init(p_gpio_tmp, pin_number, GPIO_MODER_INPUT);
   gpio_type_init(p_gpio_tmp, pin_number, GPIO_OTYPER_PUSH_PULL);
   gpio_speed_init(p_gpio_tmp, pin_number, GPIO_OSPEEDR_MEDIUM);
   gpio_pupd_init(p_gpio_tmp, pin_number, GPIO_PUPDR_NONE);
}


/*!
* @brief Reads the logic level of a single pin
* @param[in] p_gpio_tmp Pointer to GPIO structure.
* @param[in] pin_number Pin number of desired port.
* @return  pin_logic Logic value present on the desired pin.
* @warning Assumes the pin has alread been properly initialized.
* @note The operation is not atomic, watch out for possible race conditions.
*/
uint8_t
gpio_read_pin(GPIO_TypeDef * p_gpio_tmp, uint8_t pin_number)
{
   return(((p_gpio_tmp->IDR) >> pin_number) & 1ul);
}


/*** end of file ***/
