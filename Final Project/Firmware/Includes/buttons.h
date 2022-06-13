/** @file buttons.h
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

#ifndef BUTTONS_H
#define BUTTONS_H

#define BUTTON_MAIN_MODE GPIOC,0
#define BUTTON_AUTO_MODE GPIOC,1
#define BUTTON_MANUAL GPIOC,3
#define BUTTON_LED GPIOC,2

#include <stdint.h>
#include "stm32f4xx.h"
#include "stm32f410rx.h"
#include "base_gpio_drivers.h"

////////////////////////////////////////////////////////remove
#include "led.h"
#include "uart.h"
/*
****************************************************
******* Public Functions Defined in buttons.c **********
****************************************************
*/
void button_manual_init(void);
uint8_t button_manual_status(void);

void button_mode_init(void);
uint8_t button_mode_status(void);
void button_mode_reset(void);

void button_auto_init(void);
uint8_t button_auto_status(void);
void button_auto_reset(void);


void button_led_init(void);
uint8_t button_led_status(void);
void button_led_reset(void);



#endif /* BUTTONS_H */

/* end of file */
