# Exercise 2


# XMC1400 XTREME

## 1 Block Diagram
![text](https://github.com/aaronv55/Making-Embedded-Systems-Class/blob/master/Homework/Week%202/Hardware%20Block%20Diagram%20-%20XMC1400%20EXTREME.png)

## 2 Processor Core
ARM Cortex M0

## 3 Memory
200KB Flash
16KB RAM
8KB ROM

## 4 Special Peripherals
Quad SPI (not exactly groundbreaking, I know)
LIN
BCCU (brightness and color control unit) 
This puppy’s got 8 high-current pins with 50mA sink!

## 5 ADC Features
12 Channels
12-bit measurements
Adjustable gain
Input range 0 V to 5.5 V 
Out of range comparators
Internal temperature sensor
Internal/external muxable reference

## 6 Board Cost vs Processor Cost. Availability.

### KITXMC2GOXTRXMC1400TOBO1
Price: $37.38, no price breaks in volume
Very limited quantity (max 18pcs at Mouser)

### XMC1404Q064X0200AAXUMA1
Price: $6.11 in singles, $4.01 in volume
Out of stock everywhere, except dodgy websites.

## 7 Application Note
Demonstrates a fairly standard way to make a DAC from a PWM output and hardware filter.

https://www.infineon.com/dgdl/Infineon-ApplicationNote_Pseudo_Digital-to-Analog_Converter(DAC)_with_XMC1000.pdf-AN-v01_00-EN.pdf?fileId=5546d462518ffd85015255254e9c1fd3




# STM32F429I-DISC1

## 1 Block Diagram
![text](https://github.com/aaronv55/Making-Embedded-Systems-Class/blob/master/Homework/Week%202/Hardware%20Block%20Diagram%20-%20STM32F42I-DISCO.png)

## 2 Processor Core
ARM Cortex M4

## 3 Memory
2MB Flash
256KB RAM

## 4 Special Peripherals
*This thing is packed full of awesome goodies
Flexible external memory controller
LCD parallel interface
DMA with FIFO
True random number generator

## 5 ADC Features
3 separate, 12-bit ADCs
Sample rate 2.4 MSPS 
Each ADC shares 16 external channels
Simultaneous sample and hold
Internal temperature sensor
Can be tied to trigger on various timers (TIM1, TIM2, TIM3, TIM4, TIM5, or TIM8)

## 6 Board Cost vs Processor Cost. Availability.

### STM32F429I-DISC1
Price: $29.90 - $41.66, no price breaks in volume
Limited quantity (max 185pcs at Newark)

###STM32F429ZIT6
Price: $20.60 in singles, $16.35 in volume
Out of stock everywhere, except dodgy websites.

## 7 Application Note
Demonstrates various ways to use your MCU to listen to some tunes.

https://www.st.com/resource/en/application_note/an3126-audio-and-waveform-generation-using-the-dac-in-stm32-products-stmicroelectronics.pdf


