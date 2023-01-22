#include <nds.h>
#include <stdio.h>
#include "message_protocol.h"
#include "battleships.h"
#include "text.h"

int main(void) {

    
	configure_interrupts();
	init_game();
	init_audio();
	play_music();
    configure_graphics();
    load_backgrounds(MAIN_MENU);
	new_shot_sprite(1, 4,4, 1);
	new_shot_sprite(1, 5,5, 0);
	new_shot_sprite(0, 4,4, 0);
	new_shot_sprite(0, 5,5, 1);
	int wifi_text = new_text("loading wifi", 150, 180,1);
    wifi_init();
    initScores();
    fatInitDefault();
	writeScores(NULL);
    displayScores();
	update_text(wifi_text, "", -1,-1);
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
