/*
 * timers.h
 *
 *  Created on: 14 Jan 2023
 *      Author: Rafael
 */

#ifndef TIMERS_INTERRUPTS_H_
#define TIMERS_INTERRUPTS_H_
#include "battleships.h"

#define BLINK_FREQ 5

void blink_ISR();
void second_ISR();

void configure_interrupts();
void start_timer();
int is_seconds(int s);

#endif /* TIMERS_INTERRUPTS_H_ */
