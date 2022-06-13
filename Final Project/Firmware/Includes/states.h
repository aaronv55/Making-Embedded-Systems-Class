/** @file states.h
*
* @brief This file contains the logic for the system's  main state machine.
*        It defines the various menu attributes and how the system transitions
*        from one state to another.
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
*/

#ifndef STATES_H
#define STATES_H

#define STATES_MAGNETALGO_MAX 3 //Total number of different algorithms to choose from.
#define STATES_MAGNETALGO_STOP 4094 //Termination character for electromagnet algorithm lookup table

#include "buttons.h"
#include "led.h"
#include "electromagnet.h"
#include "uart.h"


/*
****************************************************
******* Public Function Defined in states.c ********
****************************************************
*/
void states_update_main_event(void);
void states_update_main_state(void);
void states_update_led(void);
void states_print_state(void);

#endif /* STATES_H */
