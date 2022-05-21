# Exercise 8



## Overview
The goal for this assignment was to familiarize ourselves with the layout of our microcontroller’s memory. In order to do so, we were required to find the addresses of various registers and variables to better understand where they are stored.
![]()


## Global Variables
![]()


## Stack Pointer
![]()
The stack pointer was the trickiest address to find out of the whole assignment. Since it lies in a dedicated register with a fixed address (that I didn’t know), I couldn’t simply declare a variable and manually check its address/contents. I spent a bit of time digging through libraries in Keil and found “cmsis_armcc.h”. This had a nifty assembly function which allowed me to read the current value with a simple call.

I then confirmed that I had received the correct address from this mystery function by comparing it to that of the local variable we just declared.


## Heap Pointer
![]()

This was another one that was slightly tricky at first glance. I’ve been told time and time again from people much smarter than me to steer clear of malloc for small embedded systems, and I therefore haven’t used it much.

The process is fairly straightforward in this case though, and malloc already provides a free pointer to find the address.


## Variables Inside Functions
![]()
