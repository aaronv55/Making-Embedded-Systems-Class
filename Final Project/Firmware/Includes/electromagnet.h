/** @file electromagnet.h
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
*
*/

#ifndef ELECTROMAGNET_H
#define ELECTROMAGNET_H

#define dac_enable() (DAC1->CR|=DAC_CR_EN1)
#define dac_disable() (DAC1->CR &= ~DAC_CR_EN1)

#define GPIO_AFRL_PIN5_MASK (0x0F << 20)
#define RCC_APB1ENR_DACEB (1ul << 29)

#define MAGNET_MAG_OFF 0
#define MAGNET_MAG_MAX 4094

#include <stdint.h>
#include "stm32f4xx.h"
#include "stm32f410rx.h"
#include "base_gpio_drivers.h"
//#include "enum_dac_volume.h"

/*
****************************************************
******* Public Functions Defined in dac.c **********
****************************************************
*/
void magnet_init(void);
void magnet_set_mag(uint16_t tmp_magnitude);


#endif /* ELECTROMAGNET_H */

/* end of file */
