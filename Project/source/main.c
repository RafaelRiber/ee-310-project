#include <nds.h>
#include <stdio.h>
#include "message_protocol.h"
#include "battleships.h"
#include "text.h"
int main(void) {

    //consoleDemoInit();
	init_ships();
	init_audio();
	play_music();
	configure_timers();
    configure_graphics();
    load_backgrounds(MAIN_MENU);
#ifndef DEBUG
    wifi_init();
#endif
	// Initialize game FSM
	GameState state = STATE_HOME; 
    while(1){
    	update_state(&state);
        update_ships();
        swiWaitForVBlank();
        oamUpdate(&oamMain);
        oamUpdate(&oamSub);
    }
}
