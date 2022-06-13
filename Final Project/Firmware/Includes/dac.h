/** @file dac.h
*
* @brief  This file contains drivers for playing audio files via the DAC
* @author Aaron Vorse
* @date   7/22/2020
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

#ifndef DAC_H
#define DAC_H

#define GPIO_AFRL_PIN5_MASK (0x0F << 20)
#define RCC_APB1ENR_DACEB (1ul << 29)
#define dac_enable() (DAC1->CR|=DAC_CR_EN1)
#define dac_disable() (DAC1->CR &= ~DAC_CR_EN1)
#define DAC_AUDIO_SHUTDOWN_PIN GPIOC,4
#define DAC_CR_TSEL1_TIM5 (3ul << 3)
#define DMA_SxCR_DIR_MEM_TO_PERIPHERAL (1ul << 6)
#define DMA_SxCR_CHSEL_CHANNEL7 (7ul << 25)

#define DAC_INACTIVE 0
#define DAC_ACTIVE 1

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
void dac_init(void);
void dac_audio_from_sd(uint32_t memory_starting_address);
uint8_t dac_service_audio(uint32_t total_blocks, uint8_t tmp_early_termination_flag);
void dac_cutoff_transmission(void);
uint32_t dac_start_audio_transmission(uint32_t tmp_address);
void dac_pause_transmission(void);


#endif /* DAC_H */

/* end of file */
