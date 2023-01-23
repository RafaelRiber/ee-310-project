// Battleships Timers and Interrupts
// EE-310 - Tharen Candi, Rafael Riber

#include "timers_interrupts.h"

int seconds;

/*
 * ISR for ship blinking. Toggles ship being hidden or not
 */
void blink_ISR(){
	player_ships[place_ship_count].is_hidden = !player_ships[place_ship_count].is_hidden;
}

/*
 * ISR for seconds counting
 */
void second_ISR() {
	seconds ++;
}

/*
 * ISR to retry sending the shots in case of packet drop due to UDP
 */
void retryShot(){
	sendMessage(SHOT, (char*) &player_target.coords); //shot is a uint8
}

/*
 * Start counting seconds, used for game over delay
 */
void start_timer() {
	seconds = 0;
	irqEnable(IRQ_TIMER1);

}

/*
 * Check if s seconds have elapsed
 */
int is_seconds(int s) {
	if (seconds >= s) {
		irqDisable(IRQ_TIMER1);
		return 1;
	}
	return 0;
}

void configure_interrupts() {
	// Configure Timer 0 for overflow interrupt
	TIMER0_CR = TIMER_ENABLE | TIMER_DIV_1024 | TIMER_IRQ_REQ;
	TIMER_DATA(0) = TIMER_FREQ_1024(BLINK_FREQ);
	// Assign ISR to timer0 interrupt
	irqSet(IRQ_TIMER0, &blink_ISR);

	// Configure Timer 1
	TIMER1_CR = TIMER_ENABLE |TIMER_DIV_1024 | TIMER_IRQ_REQ;
	TIMER_DATA(1) = TIMER_FREQ_1024(1);
	// Assign ISR for timer1 interrupt
	irqSet(IRQ_TIMER1, &second_ISR);
	seconds = 0;

	// Configure keyboard interrupt for start key
	// used to resend message in case of packet drop
	REG_KEYCNT = KEY_START | 1<<14;
	irqSet(IRQ_KEYS, &retryShot);


}
