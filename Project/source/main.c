#include <nds.h>
#include <stdio.h>
#include "message_protocol.h"
#include "battleships.h"
#include "text.h"


/*
 * BATTLESHIPS MULTIPLAYER
 * A GAME BY THAREN CANDI AND RAFAEL RIBER
 * EE-310 - WINTER SEMESTER 2022-2023
 *
 * Tested with latest libnds version as of December 2022,
 * installed with devkitPro pacman 1.0.2
 *
 */

int main(void) {

    
	configure_interrupts();
	init_game();
	init_scores();
	init_audio();
	play_music();
    configure_graphics();
    load_backgrounds(MAIN_MENU);
	int wifi_text = new_text("loading wifi", 150, 180,1);
    wifi_init();
    read_scores();
    display_scores();
    write_scores();
	update_text(wifi_text, "", -1,-1);
	// Initialize game FSM
	GameState state = STATE_HOME; 


    while(1){
    	update_state(&state);
        update_sprites();
        swiWaitForVBlank();
        oamUpdate(&oamMain);
        oamUpdate(&oamSub);
    }
}
