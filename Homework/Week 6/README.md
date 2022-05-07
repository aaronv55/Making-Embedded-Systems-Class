# Exercise 6

# Electro-Jelly Box
![text]()

## Application Overview
The Electro-Jelly Box is a small embedded system with the form factor of an iPod docking station (remember those?) It consists of a plastic housing with multiple jelly pods, each of which contain a small, silicon jellyfish. The figures are suspended in liquid and are modified to be neutrally buoyant.

The box acts as a controller, using electromagnetics to induce movement in the jellyfish and mimic natural swimming patterns. It also contains various LEDs, which are entirely for appearance. The user interacts with the device by using buttons to alter various settings like LED brightness and swim pattern. 

The device is intended to sit on a coffee table or desk, and its main goal is to give an interesting background visual, much like a lava lamp. 


## Hardware
* MCU - STM32F410RBT6
* H Bridge Module 
* Current-limiting circuit
* 12v, 200N Electromagnet
* FT232R (USB to Serial converter)
* Push buttons
* Power supply 12v, 20W

## Software

### Libraries
* Self-written STM32 drivers from previous project
* ST’s register definition & startup files

###
* Main state machine
* Electromagnet drivers
* LED drivers
* DAC drivers
* Timers
* List exact .c & .h files

## Diagrams
### Hardware Block Diagram
* In Progress - Final project deviated from initial plan

### Software Block Diagram
* In Progress - Final project deviated from initial plan

### SW Hierarchy of control diagram
* In Progress - Final project deviated from initial plan

### State Diagram
* Flow chart for readability & spreadsheet to highlight corner cases
* In Progress - Final project deviated from initial plan


## Build Instructions
### SW Toolchain
* STM32CubeIDE
* Realterm (PC application for serial communication)

### Hardware

## Future
### Position Feedback
* The system currently doesn’t have any form of location or distance sensing. This forces it to operate in an open loop configuration, which can lead to the jellyfish operating in less-than-ideal manners.

### Increased Monitoring
* Improvements can be made to ensure correct functionality and safety, such as temperature monitoring on the electromagnetic coils and input current sensing.

## Grading
N/A


## Specific to Rough Draft
###Areas where I might have trouble
* Mechanical build of jelly pods
* Algorithm to feed the electromagnet driver
* Controlling jellyfish blindly with open-loop system

### Tasks I want to complete for the project. Minimum viable project.
* Enclosure is complete
* At least one jelly pod is present
* Jellyfish is able to “swim” indefinitely without hitting either distance extreme
* LEDs illuminate the pod 

### Blocking items
* Voltage-controlled current source circuit still under development
* Assembly of jelly pods requires lots of (metaphorical) moving parts
* Complex non-linear relationship between electromagnet coil current, raw force, and force vs distance of object needs research/tuning
