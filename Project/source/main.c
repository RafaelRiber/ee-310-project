#include <nds.h>
#include <stdio.h>
#include "message_protocol.h"
#include "battleships.h"

int main(void) {

    consoleDemoInit();

    //Initialize WiFi
	wifi_init();

	// Initialize game FSM
	GameState state = STATE_INITIALIZING;


    while(1){
    	update_state(&state);
        swiWaitForVBlank();
    }
}
