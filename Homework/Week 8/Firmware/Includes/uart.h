/** @file uart.h
*
* @brief  This file contains drivers for uart initialization, and basic api commands.
* @author Aaron Vorse
* @date   3/16/2020
* @contact aaron.vorse@embeddedresume.com
*
* @note //Unless noted otherwise, br_ refers to "baud rate" in this file
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

#ifndef USART_H
#define USART_H


/****** GPIO Register Configuration Definitions ******/
#define GPIO_AF7 7ul
#define GPIO_AFRH_PIN9_AF7 (GPIO_AF7 << 4)
#define GPIO_AFRH_PIN10_AF7 (GPIO_AF7 << 8)

/************** Baud Rate Prescalers ****************/
#define BR_PRESCALER_9600_mantissa 0x28B //651.0416
#define BR_PRESCALER_9600_fraction 0x00
#define BR_PRESCALER_9600 ((BR_PRESCALER_9600_mantissa << 4) | BR_PRESCALER_9600_fraction)

#define BR_PRESCALER_19200_mantissa 0x145 //325.5208
#define BR_PRESCALER_19200_fraction 0x08
#define BR_PRESCALER_19200 ((BR_PRESCALER_19200_mantissa << 4) | BR_PRESCALER_19200_fraction)

#define BR_PRESCALER_38400_mantissa 0xA2 //162.7604
#define BR_PRESCALER_38400_fraction 0x0C
#define BR_PRESCALER_38400 ((BR_PRESCALER_38400_mantissa << 4) | BR_PRESCALER_38400_fraction)

#define BR_PRESCALER_57600_mantissa 0x6C //108.5069
#define BR_PRESCALER_57600_fraction 0x08
#define BR_PRESCALER_57600 ((BR_PRESCALER_57600_mantissa << 4) | BR_PRESCALER_57600_fraction)

#define BR_PRESCALER_74880_mantissa 0x53 //83.4668
#define BR_PRESCALER_74880_fraction 0x07
#define BR_PRESCALER_74880 ((BR_PRESCALER_74880_mantissa << 4) | BR_PRESCALER_74880_fraction)

#define BR_PRESCALER_115200_mantissa 0x36 //54.2534
#define BR_PRESCALER_115200_fraction 0x08
#define BR_PRESCALER_115200 ((BR_PRESCALER_115200_mantissa << 4) | BR_PRESCALER_115200_fraction)

#define BR_PRESCALER_230400_mantissa 0x1B //27.1267
#define BR_PRESCALER_230400_fraction 0x02
#define BR_PRESCALER_230400 ((BR_PRESCALER_230400_mantissa << 4) | BR_PRESCALER_230400_fraction)

#define BR_PRESCALER_460800_mantissa 0x0D //13.5625
#define BR_PRESCALER_460800_fraction 0x09
#define BR_PRESCALER_460800 ((BR_PRESCALER_460800_mantissa << 4) | BR_PRESCALER_460800_fraction)

#define BR_PRESCALER_921600_mantissa 0x06 //6.3781
#define BR_PRESCALER_921600_fraction 0x0C
#define BR_PRESCALER_921600 ((BR_PRESCALER_921600_mantissa << 4) | BR_PRESCALER_921600_fraction)

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "stm32f4xx.h"
#include "stm32f410rx.h"
#include "base_gpio_drivers.h"

/*
****************************************************
******* Public Functions Defined in uart.c *******
****************************************************
*/
void uart1_init(uint32_t baud_rate);
void uart1_printf(char print_statement[]);
void uart1_send_byte(char tmp_byte);
void uart1_arduino_plotter(char temp_single_char);
uint8_t uart1_is_readable(void);
char uart1_receive_byte();


#endif /* USART_H */

/* end of file */
