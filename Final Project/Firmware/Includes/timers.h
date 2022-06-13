/** @file timers.h
*
* @brief  This file contains drivers for internal timers, timer interrupts, and basic delays
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


#ifndef TIMERS_H
#define TIMERS_H

#include <stdint.h>
#include "stm32f4xx.h"
#include "stm32f410rx.h"
#include "base_gpio_drivers.h"

/*Timer Register Configuration Definitions */
#define GPIO_AF3 3ul
#define GPIO_AFRH_PIN12_AF3 (GPIO_AF3 << 16)
#define TIMER11_CHANNEL1 GPIOC, 12

#define timers_timer11_enable() TIM11->CR1 |= TIM_CR1_CEN
#define timers_timer11_disable() TIM11->CR1 &= ~TIM_CR1_CEN

#define TIMERS_TIMER5_DUTY_10 TIM11->ARR/10
#define TIMERS_TIMER5_DUTY_25 TIM11->ARR/4
#define TIMERS_TIMER5_DUTY_50 TIM11->ARR/2

#define TIM11_ARR 100UL

/*
****************************************************
******* Public Function Defined in timers.c ********
****************************************************
*/
void timers_gpio_rising_edge_pulse(GPIO_TypeDef * p_gpio_tmp, uint8_t pin_number); 
void timers_gpio_falling_edge_pulse(GPIO_TypeDef * p_gpio_tmp, uint8_t pin_number);
void timers_timer5_init(void);
void timers_timer11_init(void);
void timers_timer11_output_enable(void);
void timers_timer11_output_disable(void);
void TIM5_IRQHandler(void);
void timers_timer11_reset_status(void);
uint8_t timers_timer11_get_status(void);

uint8_t timers_tim5_get_status(void);
void timers_tim5_reset_flag(void);

void timers_delay(uint32_t delay_time);
void timers_delay_mini(uint32_t delay_time);

void timers_timer11_duty(uint16_t tmp_duty);


#endif /* TIMERS_H */

/* end of file */
