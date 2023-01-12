#include <nds.h>
#include <stdio.h>
#include "message_protocol.h"
#include "battleships.h"

int main(void) {

    //consoleDemoInit();
	init_ships();
    configure_graphics();
    load_backgrounds(MAIN_MENU);
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
