/** @file uart.c
*
* @brief  This file contains drivers for uart initialization and basic api commands.
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

#include "uart.h"

/*
****************************************************
********** Private Function Prototypes *************
****************************************************
*/

void uart1_gpio_init(void);
void uart_set_baud_rate(uint32_t temp_baud_rate);


/*
****************************************************
********** Public Function Definitions *************
****************************************************
*/

/*!
* @brief Configures USART1 in Asynchronous mode with NO hardware flow control
* @param[in] baud_rate USART1 transfer/receive rate
* @return NONE
* @warning All settings must be configured BEFORE setting the USART enable bit USART_CR1_UE
* @note See Reference Manual pages 669-679 for register configurations
*/
void uart1_init(uint32_t baud_rate)
{
   //Initialize peripheral clocks
   RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
   RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

   //Set up basic GPIO settings
   uart1_gpio_init();

   //Enable UART Transmitter and Receiver
   USART1->CR1 |= USART_CR1_TE;
   USART1->CR1 |= USART_CR1_RE;

   //Set USART Stop bit to 1
   USART1->CR2 &= ~(USART_CR2_STOP_Msk);

   //No hardware flow control
   USART1->CR3 &= ~(USART_CR3_CTSE_Msk | USART_CR3_RTSE_Msk);

   uart_set_baud_rate(baud_rate);

   //Clear these bits for asynchronous mode
   USART1->CR2 &= ~(USART_CR2_LINEN_Msk | USART_CR2_CLKEN_Msk);
   USART1->CR3 &= ~(USART_CR3_SCEN_Msk | USART_CR3_IREN_Msk | USART_CR3_HDSEL_Msk);

   //Enable USART1
   USART1->CR1 |= USART_CR1_UE;
}


/*!
* @brief Sends a string over USART i.e. printf
* @param[in] print_statement String to send
* @return NONE
*/
void
uart1_printf(char print_statement[])
{
   //Send message one char at a time
   for(uint8_t current_char = 0; current_char < strlen(print_statement); current_char++)
   {
     USART1->DR = print_statement[current_char];

     //wait for transfer complete
     uint16_t tmp_timeout = 0;

     while((0 == ((USART1->SR) & USART_SR_TC)))
     {
        tmp_timeout++;

        if(600 < tmp_timeout)
        {
           break;
        }

     }
   }
}


/*!
* @brief Sends a single byte over uart
* @param[in] tmp_byte byte to send
* @return NONE
*/
void
uart1_send_byte(char tmp_byte)
{
  USART1->DR = tmp_byte;

  //Wait for transfer complete
  while(0 == ((USART1->SR) & USART_SR_TC))
  {
  }
}


/*!
* @brief Sends a string over USART directly to Arduino Serial Plotter
* @param[in] temp_single_char Single char to be send
* @return NONE
* @note This function is only intended to interact with the "Serial Plotter" within
*       the Arduino IDE. This plotter has an abnormal API, in which ASCII characters
*       are converted to integers then displayed. For example, if the integer 123 was
*       to be displayed, USART1 would need to send char '1', then char '2', then char '3'
*/
void
uart1_arduino_plotter(char temp_single_char)
{
   //Send a single char
   USART1->DR = temp_single_char;
   while(((USART1->SR) & USART_SR_TC) == 0) {} //wait for transfer complete
   USART1->DR = ' ';
   while(((USART1->SR) & USART_SR_TC) == 0) {} //ASCII Blank space is needed for Plotter API
   USART1->DR = '\r';
   while(((USART1->SR) & USART_SR_TC) == 0) {} //Carriage Return
   USART1->DR = '\n';
   while(((USART1->SR) & USART_SR_TC) == 0) {} //New Line
}


/*
****************************************************
********** Private Function Definitions ************
****************************************************
*/

/*!
* @brief Sets the USART1 baud rate prescaler in USART1_BRR
* @param[in] temp_baud_rate Baud rate desired by user. Must be from
*                           list of rates defined in usart.h
* @return NONE
* @note The reference manual is a bit lacking, the formula is
*       prescaler = (AHB1_clk / (8 * desired_baud_rate))
*       , where the prescaler is in the format mmmmmmmmmmmm.ffff
* @warning This only applies to USART1, USART2 and others have a different scheme
*          The 8 in the above formula is only present when overclocking is set to 16
*/
void
uart_set_baud_rate(uint32_t temp_baud_rate)
{

  USART1->BRR &= ~(0xFFFF); //Clear lower two bytes of Baud Rate Register
  USART1->BRR |= temp_baud_rate;

}

/*!
* @brief Configure GPIOs PA9 and PA10 for USART1
* @param[in] NONE
* @return NONE
* @note Pins are set to Output, Alternate Function 7, Very High Speed, Push/Pull
*       See base_gpio_drivers.c for a detailed explanation
*/
void
uart1_gpio_init(void)
{
   gpio_func_init(GPIOA, 9, GPIO_MODER_ALTERNATE_FUNCTION);
   gpio_func_init(GPIOA, 10, GPIO_MODER_ALTERNATE_FUNCTION);

   gpio_speed_init(GPIOA, 9, GPIO_OSPEEDR_VERY_HIGH);
   gpio_speed_init(GPIOA, 10, GPIO_OSPEEDR_VERY_HIGH);

   gpio_type_init(GPIOA, 9, GPIO_OTYPER_PUSH_PULL);
   gpio_type_init(GPIOA, 10, GPIO_OTYPER_PUSH_PULL);

   gpio_pupd_init(GPIOA, 9, GPIO_PUPDR_PULL_UP);
   gpio_pupd_init(GPIOA, 10, GPIO_PUPDR_PULL_UP);

   GPIOA->AFR[1] |= GPIO_AFRH_PIN9_AF7;
   GPIOA->AFR[1] |= GPIO_AFRH_PIN10_AF7;
}


/*!
* @brief Check if USART1 receive data register is full
* @param[in] NONE
* @return NONE
*/
uint8_t
uart1_is_readable(void)
{
	uint8_t tmp_readable_flag = 0;

	if (USART1->SR & USART_SR_RXNE)
	{
		tmp_readable_flag = 1;
	}

	return(tmp_readable_flag);
}


/*!
* @brief Read byte from USART1
* @param[in] NONE
* @return NONE
*/
char
uart1_receive_byte(void)
{
	char tmp_char = USART1->DR; //Read byte from shadow register

	USART1->SR &= ~USART_SR_RXNE; //Clear register not empty flag

	return(tmp_char);
}

/* end of file */
