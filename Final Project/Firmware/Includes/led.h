/** @file led.h
*
* @brief  This file contains drivers for interacting with the system's leds
* @author Aaron Vorse
* @date   6/10/2022
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

#ifndef LED_H
#define LED_H

#define LED_MAG_OFF 1
#define LED_MAG_LOW TIMERS_TIMER5_DUTY_25
#define LED_MAG_MED TIMERS_TIMER5_DUTY_50
#define LED_MAG_HIGH TIM11_ARR


#include <stdint.h>
#include "timers.h"

/*
****************************************************
******* Public Functions Defined in led.c *******
****************************************************
*/
void led_set_mag(uint16_t tmp_mag);



#endif /* LED_H */

/* end of file */
