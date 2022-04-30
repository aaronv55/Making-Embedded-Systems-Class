# Exercise 5

Porting the example console code from the RPi Pico simulator to my STM32F410RB was fairly straightforward, although there were a few hiccups. I’m glad I didn’t have to write the scary-looking command buffer and string comparison routine.

I reused a bare-bones UART library that I developed a year ago, and most of my effort went into creating a wrapper in the “consoleIo.c” file.


![text](https://github.com/aaronv55/Making-Embedded-Systems-Class/blob/master/Homework/Week%205/Console%20UART%20Wrapper.png)


I rewrote the internals of the send and receive functions used by the main console file to run off of the STM32’s UART instead of the simulator’s PC console. Here’s where things fell apart a bit…

I spent an embarrassingly long amount of time feverishly throwing commands at my board from a PC terminal, wondering why it wouldn’t respond. I even hard coded a command directly in the parsing function to try and get it running. After reading through the entire “console.c” file, I realized the function was expecting a line feed or carriage return as a delimiter.


![text](https://github.com/aaronv55/Making-Embedded-Systems-Class/blob/master/Homework/Week%205/Command%20List%20Additions.png)


After including LF/CR in my send messages, everything started working perfectly. I added a few commands to the list. I know these aren’t exactly groundbreaking, but they’re mostly meant as place holders. I plan to add more useful and complex commands as I develop the final project. 

![text](https://github.com/aaronv55/Making-Embedded-Systems-Class/blob/master/Homework/Week%205/Command%20Help.png)

![text](https://github.com/aaronv55/Making-Embedded-Systems-Class/blob/master/Homework/Week%205/Command%20Multiple.png)