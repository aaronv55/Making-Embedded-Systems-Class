/** @file system_clock.C
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
*
*/

#include "system_clock.h"

/*
****************************************************
********** Private Function Prototypes *************
****************************************************
*/
void ahb_prescaler_init(void);
void apb1_init(void);
void apb2_init(void);
void apb_prescaler_init(void);
void flash_init(void);
void voltage_scale_init(void);
void hsi_init(void);
void pll_init(void);
void system_tick_init(uint32_t num_ticks);

/*
****************************************************
********** Public Function Definitions *************
****************************************************
*/


/*!
* @brief Configure system clock to 100MHz and peripheral clocks
* @param[in] NONE
* @return NONE
* @note Options include
*/
void
system_clock_init(void)
{
   apb2_init();
   apb1_init();
   flash_init();
   voltage_scale_init();
   hsi_init();
   pll_init();
   ahb_prescaler_init();
   apb_prescaler_init();
   system_tick_init(1000UL);
   RCC->CFGR |= (RCC_CFGR_MCO1EN | RCC_CFGR_MCO2EN);
}

/*
****************************************************
********** Private Function Definitions ************
****************************************************
*/

/*!
* @brief Enable APB1
* @param[in] NONE
* @return NONE
*/
void
apb1_init(void)
{
   RCC->APB1ENR |= RCC_APB1ENR_PWREN; //2
}


/*!
* @brief Enable APB2
* @param[in] NONE
* @return NONE
* @note This is used by USART1
*/
void
apb2_init(void)
{
   RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; //1
}


/*!
* @brief Set Flash wait states
* @param[in] NONE
* @return NONE
* @note Three wait states are recommended by the reference manual
*       for a system clock of 100MHz
*/
void
flash_init(void)
{
   FLASH->ACR &= ~(0XF); //4
   FLASH->ACR |= FLASH_ACR_LATENCY_3WS;
}


/*!
* @brief Set the internal voltage regulator mode
* @param[in] NONE
* @return NONE
* @warning The PWR_CR register can only be modified when
*          PLL is OFF.
*/
void
voltage_scale_init(void)
{
   //This must be set to Scale 1 (3ul) for a system clock of 100MHz
   PWR->CR |= PWR_CR_VOS_SCALE1;
}


/*!
* @brief Set the HSI Calibration
* @param[in] NONE
* @return NONE
* @note HSI Trim may need to be adjusted for a different IC
*       based on its factory HSICAL value
*/
void
hsi_init(void)
{
   RCC->CR &= ~(RCC_CR_HSI_TRIMM_Msk); //Clear HSI_Trim
   RCC->CR |= (0X10 << 3); //Set HSI Trim to 16

   RCC->CR |= (RCC_CR_HSION); //Enable HSI

   while((RCC->CR) & !RCC_CR_HSIRDY) //Wait until HSI is ready
   {
   }

}


/*!
* @brief Configure PLL prescalers
* @param[in] NONE
* @return NONE
* @note Formulas from the reference manual
*       � f(VCO clock) = f(PLL clock input) * (PLLN / PLLM)
*       � f(PLL general clock output) = f(VCO clock) / PLLP
*       � f(I2S, System) = f(VCO clock) / PLLR
*/
void
pll_init(void)
{

   RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLSRC_Msk); //Set PLL source to HSI


   RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLM_Msk); //Configure PLLM Prescaler
   RCC->PLLCFGR |= (8ul << RCC_PLLCFGR_PLLM_Pos);

   RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLN_Msk); //Configure PLLN Prescaler
   RCC->PLLCFGR |= (100ul << RCC_PLLCFGR_PLLN_Pos);

   RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLP_Msk); //Configure PLLP Prescaler

   RCC->CR |= RCC_CR_PLLON; //Enable PLL
   while((RCC->CR) & !RCC_CR_PLLRDY)
   {
   }
}


/*!
* @brief Set AHB Prescaler
* @param[in] NONE
* @return NONE
* @note From reference manual
* 1000: system clock divided by 2
* 1001: system clock divided by 4
* 1010: system clock divided by 8
* 1011: system clock divided by 16
* 1100: system clock divided by 64
* 1101: system clock divided by 128
* 1110: system clock divided by 256
* 1111: system clock divided by 512
*/
void
ahb_prescaler_init(void)
{
   RCC->CFGR &= ~(RCC_CFGR_HPRE_Msk); //No system clock division
}


/*!
* @brief Set APB Prescaler
* @param[in] NONE
* @return NONE
* @note From reference manual
* For both PREP1 and PPRE2:
* 0xx: AHB clock not divided
* 100: AHB clock divided by 2
* 101: AHB clock divided by 4
* 110: AHB clock divided by 8
* 111: AHB clock divided by 16
*
*/
void
apb_prescaler_init(void)
{
   RCC->CFGR &= ~(RCC_CFGR_PPRE1_Msk);
   RCC->CFGR |= (RCC_CFGR_PPRE1_DIV2); //APB Low speed = AHB/2

   RCC->CFGR &= ~(RCC_CFGR_PPRE2_Msk); //APB High speed NOT divided

   RCC->CFGR &= ~(RCC_CFGR_SW_Msk);
   RCC->CFGR |= (RCC_CFGR_SW_PLL);//PLL selected as system clock
}


/*!
* @brief Set system tick to 1ms
* @param[in] NONE
* @return NONE
* @warning This assumes SYSTEMCLOCK_FREQUENCY is 100MHz
*/
void
system_tick_init(uint32_t num_ticks)
{
   uint32_t temp_load_val = ((SYSTEM_CLOCK_FREQUENCY / num_ticks) - 1UL);
   SysTick->LOAD = temp_load_val;
   SysTick->VAL = 0UL;
   SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;

}


/* end of file */
