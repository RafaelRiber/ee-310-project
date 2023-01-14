/*
 * timers.h
 *
 *  Created on: 14 Jan 2023
 *      Author: Rafael
 */

#ifndef TIMERS_INTERRUPTS_H_
#define TIMERS_INTERRUPTS_H_
#include "battleships.h"

#define BLINK_FREQ 4

void blink_interrupt();

void configure_timers();


#endif /* TIMERS_INTERRUPTS_H_ */
