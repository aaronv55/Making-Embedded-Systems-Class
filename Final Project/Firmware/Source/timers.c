/** @file timers.c
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
*
*/

#include "timers.h"



/*
****************************************************
************* File-Static Variables ****************
****************************************************
*/
static uint8_t tim5_interrupt_flag = 0;


/*
****************************************************
********** Private Function Prototypes *************
****************************************************
*/
void timer_timer11_gpio_init(void);

/*
****************************************************
********** Public Function Definitions *************
****************************************************
*/


/*!
* @brief Initialize timer 5 to
* @param[in]
* @return  NONE

*/
void
timers_timer5_init(void)
{
   //Enable main timer clock
   RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;

   //Fill timer prescale registers. Formula: total_prescale = (PSC x ARR)
   TIM5->PSC = 9999;
   TIM5->ARR = 1000;

   //Set timer overflow as interrupt source
   TIM5->CR1 |= TIM_CR1_URS;

   //Enable timer interrupts
   TIM5->DIER |= TIM_DIER_UIE;

   //Set timer to trigger output. This is needed to trigger DMA
   TIM5->CR2 |= TIM_CR2_MMS_1;

   //Push setting changes to timer5
   TIM5->EGR |= TIM_EGR_UG;

   //Initialize timer5 ISR
   NVIC_EnableIRQ(TIM5_IRQn);

   //Enable timer
   TIM5->CR1 |= TIM_CR1_CEN;
}


/*!
* @brief Adjusts the frequency of timer 5 on the fly
* @param[in] tmp_frequency
* @return  NONE
*/
void
timers_timer5_freq(uint32_t tmp_frequency)
{
   //Disable timer
   TIM5->CR1 &= ~TIM_CR1_CEN;

   //Fill timer prescale registers. Formula: total_prescale = (PSC x ARR)
   TIM5->PSC = 1000;
   TIM5->ARR = tmp_frequency;

   //Set timer overflow as interrupt source
   TIM5->CR1 |= TIM_CR1_URS;

   //Enable timer interrupts
   TIM5->DIER |= TIM_DIER_UIE;

   //Set timer to trigger output. This is needed to trigger DMA
   TIM5->CR2 |= TIM_CR2_MMS_1;

   //Push setting changes to timer5
   TIM5->EGR |= TIM_EGR_UG;

   //Enable timer
   TIM5->CR1 |= TIM_CR1_CEN;
}

/*!
* @brief Initialize timer 11 to 100mS, PWM mode
* @param[in] 
* @return  NONE
*/
void
timers_timer11_init(void)
{

   RCC->APB2ENR |= RCC_APB2ENR_TIM11EN; //Enable APB2 clock
   timer_timer11_gpio_init();
   TIM11->ARR = TIM11_ARR; //Set Autoreload
   TIM11->PSC = 9999UL; //Initialize Prescaler

   TIM11->EGR = TIM_EGR_UG; //Update Event
   TIM11->CCER |= TIM_CCER_CC1E; //Enable Channel 1 output
   TIM11->CCMR1 |=  TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;//Polarity of duty cycle
   TIM11->CCR1 = TIM11->ARR / 2;  //Set initial duty cycle to 50%

   TIM11->CCMR1 |= TIM_CCMR1_OC1PE; //Enable Preload register

   timers_timer11_enable();
   timers_timer11_output_enable();

   TIM11->EGR |= TIM_EGR_UG;  //Push setting changes to timer11
}

/*!
* @brief Adjusts the duty cycle of timer 11 on the fly
* @param[in] tmp_frequency
* @return  NONE
*/
void
timers_timer11_duty(uint16_t tmp_duty)
{
   if(0 != tmp_duty)
   {
      TIM11->CCR1 = tmp_duty;
      TIM11->EGR = TIM_EGR_UG; //Update Event
      timers_delay_mini(100);
   }
}




/*!
* @brief Check if the timer 5 interrupt has triggered
* @param[in] NONE
* @return  tim5_interrupt_flag
*/
uint8_t
timers_tim5_get_status(void)
{
   return(tim5_interrupt_flag);
}



/*!
* @brief
* @return  NONE
*/
void
timers_tim5_reset_flag(void)
{
   tim5_interrupt_flag = 0;
}


/*!
* @brief Enables Timer11 channel 1 output, which pushes the current timer logic value to a specified GPIO
* @param[in] NONE
* @return  NONE
*/
void
timers_timer11_output_enable(void)
{
   TIM11->CCER |= TIM_CCER_CC1E;   
}
   
/*!
* @brief Disables Timer11 channel 1 output
* @param[in] NONE
* @return  NONE
* @note If GPIO is open drain, output of channel 1
*       will go into high z mode. 
*/
void
timers_timer11_output_disable(void)
{
   TIM11->CCER &= ~TIM_CCER_CC1E; 
}


/*!
* @brief Bad delay that locks the processor
* @param[in] NONE
* @return  NONE
*/
void
timers_delay(uint32_t delay_time)
{
   uint32_t i = 0;
   for(i = 0; i < (delay_time * 1000); i++) {} //lock MCU until delay time finishes
}


/*!
* @brief Bad delay that locks the processor
* @param[in] NONE
* @return  NONE
*/
void
timers_delay_mini(uint32_t delay_time)
{
   uint32_t i = 0;
   for(i = 0; i < (delay_time);i++) {} //lock MCU until delay time finishes
}


/* Private Function Definitions */

/*!
* @brief Initialize PC12 as Timer11 channel1 output
* @param[in] NONE
* @return  NONE
*/
void
timer_timer11_gpio_init(void)
{
   gpio_clk_init(TIMER11_CHANNEL1);
   gpio_func_init(TIMER11_CHANNEL1, GPIO_MODER_ALTERNATE_FUNCTION);
   gpio_type_init(TIMER11_CHANNEL1, GPIO_OTYPER_PUSH_PULL);
   gpio_speed_init(TIMER11_CHANNEL1, GPIO_OSPEEDR_VERY_HIGH);
   gpio_pupd_init(TIMER11_CHANNEL1, GPIO_PUPDR_NONE);
   GPIOC->AFR[1] |= GPIO_AFRH_PIN12_AF3; 
}

/*
* @brief Timer 5 interrupt handler
* @param[in] NONE
* @return  NONE
*/
void
TIM5_IRQHandler(void)
{
   tim5_interrupt_flag = 1;

   TIM5->SR &= ~TIM_SR_UIF;
}


/* end of file */
