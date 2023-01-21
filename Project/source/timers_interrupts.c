/*
 * timers.c
 *
 *  Created on: 14 Jan 2023
 *      Author: Rafael
 */
#include "timers_interrupts.h"

int seconds;

void blink_ISR(){
	player_ships[place_ship_count].is_hidden = !player_ships[place_ship_count].is_hidden;
}
void second_ISR() {
	seconds ++;
}

void retryShot(){
	sendMessage(SHOT, (char*) &player_target.coords); //shot is a uint8
}

void start_timer() {
	seconds = 0;
	irqEnable(IRQ_TIMER1);

}
int is_seconds(int s) {
	if (seconds >= s) {
		irqDisable(IRQ_TIMER1);
		return 1;
	}
	return 0;
}

void configure_interrupts() {

	TIMER0_CR = TIMER_ENABLE | TIMER_DIV_1024 | TIMER_IRQ_REQ;
	TIMER_DATA(0) = TIMER_FREQ_1024(BLINK_FREQ);
	irqSet(IRQ_TIMER0, &blink_ISR);

	TIMER1_CR = TIMER_ENABLE |TIMER_DIV_1024 | TIMER_IRQ_REQ;
	TIMER_DATA(1) = TIMER_FREQ_1024(1);
	irqSet(IRQ_TIMER1, &second_ISR);
	seconds = 0;

	REG_KEYCNT = KEY_START | 1<<14;
	irqSet(IRQ_KEYS, &retryShot);


}
