# Exercise 9

The goal of this assignment is to implement a small algorithm in a number of different ways and weigh their pros and cons. I chose to create an algorithm to convert unsigned, 8-bit integers to an ASCII string.


## Method 1: Sprintf()
![](https://github.com/aaronv55/Making-Embedded-Systems-Class/blob/master/Homework/Week%209/Code_Method_Sprintf.png)

The main benefit of using this method is that it’s arguably the most straightforward solution. It makes your codebase much more portable and easier for yourself and others to understand.

The downside is that you will have to deal with the overhead of including sprintf(). This comes in the form of both undesired latency and added code size from the function itself and its dependencies.

## Method 2: Lookup Table
![](https://github.com/aaronv55/Making-Embedded-Systems-Class/blob/master/Homework/Week%209/Code_Method_LookupTable.png)

This method maps a uint8 to one of 256 possible string combinations. Although there is most likely a much faster way than strcpy() to copy the desired element to tmp_str, using a lookup table like this is generally one of the faster ways to implement algorithms.

The downfall here is that you’re going to have to trade all that speed for code space. A char array of 256 x 4 elements will burn a large amount of memory. If we wanted to convert a larger data type (uint32_t for example), the memory required would quickly surpass that available in the system.


## Method 3: Division (Don’t do this)
![](https://github.com/aaronv55/Making-Embedded-Systems-Class/blob/master/Homework/Week%209/Code_Method_Division.png)

A few years ago in a fit of desperation (before I knew about sprinft), I actually implemented this method while trying to debug over a serial terminal. I can’t stress enough how bad of an idea this is. Essentially, division is used to find the value of each tens place, which is then added to the ASCII number ‘0’. Afterwards, the value of that tens place is subtracted from the original number, and the process repeats until there are no more digits. I told you this was a bad idea.

The main downfall here is that this thing is slow, very slow. Each tens place takes many instructions to handle, and the function becomes much larger than it needs to be. Using godbolt.org, I was able to see that this method required nearly three times the instructions to execute as the other versions. 
