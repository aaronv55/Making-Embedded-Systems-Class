/** @file base_gpio_drivers.h
*
* @brief  Drivers for GPIO initialization, and basic pin manipulation
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
*
*/

#ifndef BASE_GPIO_DRIVERS_H
#define BASE_GPIO_DRIVERS_H

/*
****************************************************
*** GPIO Register Configuration DefinitionsPublic **
****************************************************
*/
#define GPIO_MODER_INPUT                0x00 //00000000   This is the default on reset.
#define GPIO_MODER_GENERAL_OUTPUT       0x01 //00000001
#define GPIO_MODER_ALTERNATE_FUNCTION   0x02 //00000010
#define GPIO_MODER_ANALOG               0x03 //00000011
#define GPIO_OTYPER_PUSH_PULL           0x00 //00000000   This is the default on reset.
#define GPIO_OTYPER_OPEN_DRAIN          0x01 //00000001
#define GPIO_OSPEEDR_LOW                0x00 //00000000
#define GPIO_OSPEEDR_MEDIUM             0x01 //00000001
#define GPIO_OSPEEDR_HIGH               0x02 //00000010
#define GPIO_OSPEEDR_VERY_HIGH          0x03 //00000011
#define GPIO_PUPDR_NONE                 0x00 //00000000
#define GPIO_PUPDR_PULL_UP              0x01 //00000001
#define GPIO_PUPDR_PULL_DOWN            0x02 //00000010

#include <stdint.h>
#include "stm32f4xx.h"
#include "stm32f410rx.h"

/*
****************************************************
** Public Function Defined in base_gpio_drivers.c **
****************************************************
*/
void    gpio_clk_init(GPIO_TypeDef * p_gpio_tmp, uint8_t pin_number);
void    gpio_func_init(GPIO_TypeDef * p_gpio_tmp, uint8_t pin_number, uint8_t pin_func);
void    gpio_type_init(GPIO_TypeDef * p_gpio_tmp, uint8_t pin_number, uint8_t pin_type);
void    gpio_speed_init(GPIO_TypeDef * p_gpio_tmp, uint8_t pin_number, uint8_t pin_speed);
void    gpio_pupd_init(GPIO_TypeDef * p_gpio_tmp, uint8_t pin_number, uint8_t pin_pupd);
void    gpio_set(GPIO_TypeDef * p_gpio_tmp, uint8_t pin_number);
void    gpio_clear(GPIO_TypeDef * p_gpio_tmp, uint8_t pin_number);
void    gpio_gen_output_init(GPIO_TypeDef * p_gpio_tmp, uint8_t pin_number);
void    gpio_gen_input_init(GPIO_TypeDef * p_gpio_tmp, uint8_t pin_number);
uint8_t gpio_read_pin(GPIO_TypeDef * p_gpio_tmp, uint8_t pin_number);

#endif /* BASE_GPIO_DRIVERS_H */

/*** end of file ***/
