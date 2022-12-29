#include <nds.h>
#include <stdio.h>
#include "message_protocol.h"
#include "battleships.h"

int main(void) {

    consoleDemoInit();


	// Initialize game FSM
	GameState state = STATE_HOME;

    while(1){
    	update_state(&state);
        swiWaitForVBlank();
    }
}
