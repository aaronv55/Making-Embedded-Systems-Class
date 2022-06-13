/** @file system_clock.h
*
* @brief  This file contains drivers to initialize the main clock of the MCU, along with its peripherals and parts of the RTC
* @author Aaron Vorse
* @date   3/31/2020
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

#ifndef SYSTEM_CLOCK_H
#define SYSTEM_CLOCK_H

#define SYSTEM_CLOCK_FREQUENCY 100000000UL //100MHz
#define PWR_CR_VOS_SCALE1 0X0000C000U;
#define RCC_CR_HSI_TRIMM_Msk (0X1F << 3UL)

#include <stdint.h>
#include "stm32f4xx.h"
#include "stm32f410rx.h"

/*
****************************************************
***** Public Function Defined in sysem_clock.c *****
****************************************************
*/
void system_clock_init(void);


#endif /* SYSTEM_CLOCK_H */

/* end of file */
