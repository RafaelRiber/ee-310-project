#include <nds.h>
#include <stdio.h>
#include "message_protocol.h"
#include "battleships.h"

int main(void) {

    //consoleDemoInit();
	init_ships();
    configure_graphics();
    new_shot_sprite(1, 3, 3);
    new_shot_sprite(0, 5, 5);
    set_target_coords(2,2);
    player_target.is_hidden = 0;
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
