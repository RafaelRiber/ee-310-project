#include <nds.h>
#include <stdio.h>
#include "message_protocol.h"
#include "battleships.h"

int main(void) {

    consoleDemoInit();

    //Initialize WiFi
	wifi_init();
    while(1){
        swiWaitForVBlank();
    }
}
