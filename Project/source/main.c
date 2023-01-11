#include <nds.h>
#include <stdio.h>
#include "message_protocol.h"
#include "battleships.h"
#include "graphics.h"

int main(void) {

    //consoleDemoInit();
	init_ships();
    configure_graphics();
    load_backgrounds(GAME);

	// Initialize game FSM
	GameState state = STATE_HOME;
    new_shot_sprite(1, 5, 4);
    int x,y;
    x = 0;
    y = 0;
    while(1){
    	update_state(&state);
        update_ships();
        swiWaitForVBlank();
        oamUpdate(&oamMain);
        move_ship(&player_ships[CARRIER],x, y, 1);

        if(x > 10 - player_ships[CARRIER].len) {
            x = 0;
            y ++;
        }
        x ++;
        if (y > 10) {
            y = 0;
            x = 0;
        }
        int i;
        //delay
        for (i = 0; i < 10000; i ++)

        oamUpdate(&oamSub);
        
    }
}
