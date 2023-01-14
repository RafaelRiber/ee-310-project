/*
 * timers.c
 *
 *  Created on: 14 Jan 2023
 *      Author: Rafael
 */
#include "timers_interrupts.h"

void blink_interrupt(){
	//player_ships[place_ship_count].is_hidden = !player_ships[place_ship_count].is_hidden;
}

void configure_timers() {
	TIMER0_CR = TIMER_ENABLE | TIMER_DIV_1024 | TIMER_IRQ_REQ;
	TIMER_DATA(0) = TIMER_FREQ_1024(BLINK_FREQ);
	irqSet(IRQ_TIMER0, &blink_interrupt);
}
