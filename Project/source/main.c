#include <nds.h>
#include <stdio.h>
#include "message_protocol.h"
#include "battleships.h"

int main(void) {

    consoleDemoInit();

    //Initialize WiFi
	wifi_init();
    while(1){
    	//Send a message if key is pressed
		sendMessageTest();
		//Receive message of someone else that pressed a key
		receiveMessageTest();

        swiWaitForVBlank();
    }
}
