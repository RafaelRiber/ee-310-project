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
	configure_interrupts();
    configure_graphics();
    load_backgrounds(MAIN_MENU);
#ifndef DEBUG
	int wifi_text = new_text("loading wifi", 150, 180,1);
    wifi_init();
    initScores();
    fatInitDefault();
	writeScores(NULL);
    displayScores();
	update_text(wifi_text, "", -1,-1);
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
