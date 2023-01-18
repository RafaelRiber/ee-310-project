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
	int tid = new_text("test text", 100,100, 0);
	int i = 100;
	int j = 100;
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
		update_text(tid, "test updated", i,j);
		i += 8;
		j += 8;
		if (i > 150 || j > 150) {
			i = 100;
			j = 100;
		}

		for (int x = 0; x < 1000000; x ++);

    }
}
