/** @file dac.c
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
*/

#include "dac.h"

/*
****************************************************
************* File-Static Variables ****************
****************************************************
*/



/*
****************************************************
********** Private Function Prototypes *************
****************************************************
*/
void dac_dma1_init(void);
void dac_set_audio_size(uint32_t tmp_size);


/*
****************************************************
********** Public Function Definitions *************
****************************************************
*/

/*!
* @brief Initializes the DAC
* @param[in] NONE
* @return NONE
*/
void
dac_init(void)
{
   //Initialize peripheral clocks
   RCC->APB1ENR |= RCC_APB1ENR_DACEB;
   RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
   
   //Set up basic GPIO settings
   gpio_func_init(GPIOA, 5, GPIO_MODER_ALTERNATE_FUNCTION);
   gpio_speed_init(GPIOA, 5, GPIO_OSPEEDR_VERY_HIGH);
   gpio_type_init(GPIOA, 5, GPIO_OTYPER_PUSH_PULL);
   gpio_pupd_init(GPIOA, 5, GPIO_PUPDR_NONE);
   GPIOA->AFR[0] &= ~(GPIO_AFRL_PIN5_MASK);
   
   
   //Enable DAC1
   DAC1->CR |= DAC_CR_EN1;

   //Set initial voltage
   DAC1->DHR12R1 |= 0x00FF;



   //DAC trigger set to external, TIM5
   //DAC1->CR |= (DAC_CR_TEN1 | DAC_CR_TSEL1_TIM5 );
   
   //Enable the interrupt on transfer ISR
   //NVIC_EnableIRQ(DMA1_Stream5_IRQn);

   //Enable the DAC and DMA
   //dac_dma1_init();
   //dac_disable_audio();

}


/*!
* @brief Sends sound clip from SD card to LCD 
* @param[in] memory_starting_address Address block in the SD card where the audio file is located
* @return NONE
*
*/
void
dac_audio_from_sd(uint32_t memory_starting_address)
{

   uint16_t current_byte = 0;
   uint32_t total_blocks = 0, block_number = 0;
   
   uint16_t buffer_ping[512] = {0}, buffer_pong[512] = {0};

   uint16_t *p_current_buffer = buffer_ping;
   uint16_t *p_dormant_buffer = buffer_pong;
   uint16_t *p_tmp_swap_placeholder = NULL;
   
   dac_enable();

   //512 bytes-per-block
   total_blocks = dac_start_audio_transmission(memory_starting_address);
   timers_delay(200);

   for(block_number = 0; block_number < (total_blocks); block_number++)
   {

      //Wait for DMA to transfer the contents of current buffer
      //while(g_dma_transfer_complete_flag == 0) {};

      //Toggle current buffer
      p_tmp_swap_placeholder = p_current_buffer;
      p_current_buffer = p_dormant_buffer;
      p_dormant_buffer = p_tmp_swap_placeholder;

      //g_dma_transfer_complete_flag = 0;

      DMA1_Stream5->NDTR = 512;
      DMA1_Stream5->M0AR = (uint32_t)p_current_buffer;
      DMA1_Stream5->CR |= DMA_SxCR_EN;

      //Burn through the CRC bits and wait until SD card sends data valid token
      ///while(spi_receive_byte(0xFF) != SD_CMD17_TOKEN); //CMD17 token is the same as CMD18, 0xFE

      for(current_byte = 0; current_byte < 512; current_byte++)
      {
         *(p_dormant_buffer + current_byte) = (uint16_t)(spi_receive_byte(0xFF));
      }

   }
   
   //Disable DMA and timer 5
   TIM5->CR1 &= ~TIM_CR1_CEN;
   DMA1_Stream5->CR &= ~DMA_SxCR_EN;

   //The sd card must return an entire block. Flush the unused bytes from the last block
   sd_stop_transmission();

   dac_disable();
}


/*!
* @brief Sends sound clip from SD card to LCD
* @param[in] total_blocks Total memory blocks in the SD card that the audio file takes up
* @param[in] tmp_early_termination_flag This tells the function whether the last audio was
*                      cut off early and the "current block number" needs to be reset
* @return transmission_complete A flag used to tell the audio state machine that the
*                               current sound clip has ended
*/
uint8_t
dac_service_audio(uint32_t total_blocks, uint8_t tmp_early_termination_flag)
{
   //Enable the DMA -> DAC clock and timer11 in case the audio just came out of
   // a paused state
   dac_enable();
   TIM5->CR1 |= TIM_CR1_CEN;
   timers_timer11_enable();

   static uint16_t buffer_ping[512] = {0};
   static uint32_t block_number = 0;
   static uint16_t buffer_pong[512] = {0};
   static uint16_t *p_current_buffer = buffer_ping;
   static uint16_t *p_dormant_buffer = buffer_pong;
   static uint16_t *p_tmp_swap_placeholder = NULL;

   uint8_t transmission_complete = 0;

   //If the last audio broke before it was done playing, block number needs to be reset so
   // that the current audio plays for its full length
   if(tmp_early_termination_flag)
   {
      block_number = 0;
   }

   if(block_number < total_blocks)
   {

      //Wait for DMA to transfer the contents of current buffer
      //while(0 == g_dma_transfer_complete_flag) {}

      //Toggle current buffer
      p_tmp_swap_placeholder = p_current_buffer;
      p_current_buffer = p_dormant_buffer;
      p_dormant_buffer = p_tmp_swap_placeholder;

      //g_dma_transfer_complete_flag = 0;

      //Reinitialize the DMA with the new buffer address and enable it
      DMA1_Stream5->NDTR = 512;
      DMA1_Stream5->M0AR = (uint32_t)p_current_buffer;
      DMA1_Stream5->CR |= DMA_SxCR_EN;

      //Burn through the CRC bits and wait until SD card sends data valid token
      //while(SD_CMD17_TOKEN != spi_receive_byte(0xFF)) {} //CMD17 token is the same as CMD18, 0xFE

      uint8_t tmp_volume = dac_get_volume();

      //Fill the inactive buffer with the next block of data
      for(uint16_t current_byte = 0; current_byte < 512; current_byte++)
      {
         *(p_dormant_buffer + current_byte) = (((uint16_t)(spi_receive_byte(0xFF))) << tmp_volume);
      }

      block_number++;
   }

   else
   {
      block_number = 0;
      transmission_complete = 1;
   }

   return(transmission_complete);

}




/*!
* @brief Safely stops the current audio transmission without locking the SD card
* @param[in] NONE
* @return NONE
*/
void
dac_cutoff_transmission(void)
{
   uint16_t timeout = 5000;



   //Disable DMA and timer 5
   TIM5->CR1 &= ~TIM_CR1_CEN;
   DMA1_Stream5->CR &= ~DMA_SxCR_EN;

   //The sd card must return an entire block. Flush the unused bytes from the last block
   sd_stop_transmission();
   dac_disable();
}


/*!
* @brief Stops the clock which is feeding the DMA into the dac. The DMA and its
*        current buffer are retained and resume as soon as the clock is enabled
* @param[in] NONE
* @return NONE
*
*/
void
dac_pause_transmission(void)
{
   //Disable Timer 5
   TIM5->CR1 &= ~TIM_CR1_CEN;
}

/*
****************************************************
********** Private Function Definitions ************
****************************************************
*/

/*!
* @brief Initializes DMA1 to trigger from timer5 and output to the DAC
* @param[in] NONE
* @return NONE
*
*/
void
dac_dma1_init(void)
{
   //Enable main DMA clock
   RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;

   //Couple DAC to DMA
   DAC->CR |= DAC_CR_DMAEN1;

   // Set DMA to increment memory, interrupt on transfer complete, circular mode,
   //direction: memory to peripheral, channel 7 select, 512-byte transfers.
   DMA1_Stream5->NDTR = 512;
   DMA1_Stream5->CR |= (DMA_SxCR_MINC | DMA_SxCR_TCIE | DMA_SxCR_DIR_MEM_TO_PERIPHERAL | DMA_SxCR_CHSEL_CHANNEL7);

   //Source and destination registers are 16bit
   DMA1_Stream5->CR |= (DMA_SxCR_MSIZE_0 | DMA_SxCR_PSIZE_0);

   //Send to 12-bit right aligned DAC output register
   DMA1_Stream5->PAR = (uint32_t)&(DAC1->DHR12R1);

}


/*!
* @brief ISR to handle DMA transfer complete interrupt
* @param[in] NONE
* @return NONE
*
*/
void
DMA1_Stream5_IRQHandler(void)
{
   //g_dma_transfer_complete_flag = 1;

   //Clear interrupt flag
   DMA1->HIFCR = DMA_HIFCR_CTCIF5;

   //Disable DMA
   DMA1_Stream5->CR &= ~DMA_SxCR_EN;
}


/*!
* @brief Does the initial handshake with the SD card and loads the first block into
*        the ping buffer. It also checks the WAV header for file size.
* @param[in] tmp_address Address block in the SD card where the audio file is located
* @return file_size Size of WAV file in blocks, which is size in bytes divided by 512
*                   bytes per block
*/
uint32_t
dac_start_audio_transmission(uint32_t tmp_address)
{
   uint16_t tmp_current_byte = 0;
   uint32_t file_size = 0; //Size of WAV in blocks

   dac_dma1_init();

   //Start reading the WAV file from the SD card
   //sd_read_multiple_block(tmp_address);

   //Jump to file size section of header.
   spi_receive_byte(0xFF);
   spi_receive_byte(0xFF);
   spi_receive_byte(0xFF);
   spi_receive_byte(0xFF);

   //Read file size from header. Warning, file size is little endian in WAV header
   file_size = (uint32_t)(spi_receive_byte(0xFF));
   file_size |= (((uint32_t)(spi_receive_byte(0xFF))) << 8);
   file_size |= (((uint32_t)(spi_receive_byte(0xFF))) << 16);
   file_size |= (((uint32_t)(spi_receive_byte(0xFF))) << 24);

   //Jump to the first byte of actual sound data
   while(tmp_current_byte < 35)
   {
      spi_receive_byte(0xFF);
      tmp_current_byte++;
   }

   uint16_t tmp_buffer[512] = {0};

   uint8_t tmp_volume = dac_get_volume();

   //Fill buffer with first block, excluding the header
   while(tmp_current_byte < 512)
   {
      *(tmp_buffer + tmp_current_byte) = (((uint16_t)spi_receive_byte(0xFF)) << tmp_volume);
      tmp_current_byte++;
   }

   //Enable DMA
   DMA1->HIFCR = 0x0F7C0F7C;
   DMA1_Stream5->NDTR = 512;
   DMA1_Stream5->M0AR = (uint32_t)tmp_buffer; //DMA read from ping buffer
   DMA1_Stream5->CR |= DMA_SxCR_EN;

   //Enable timer
   TIM5->CR1 |= TIM_CR1_CEN;

   //Wait for DMA to transfer the contents of current buffer
   //while(g_dma_transfer_complete_flag == 0) {};

   //512 bytes-per-block
   return(file_size / 512);
}


/* end of file */
