#include <nds.h>
#include <stdio.h>
#include "message_protocol.h"
#include "battleships.h"
#include "graphics.h"

int main(void) {

    consoleDemoInit();
	init_ships();
    configure_graphics();

    //Initialize WiFi
	//wifi_init();
    //Position
	printf("hello\n");
    while(1){

		
        update_sprites();
    	swiWaitForVBlank();
    	
    
        oamUpdate(&oamMain);

    }
}
