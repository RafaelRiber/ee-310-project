#include <nds.h>
#include <stdio.h>
#include "message_protocol.h"
#include "battleships.h"

int main(void) {

    consoleDemoInit();

    //Initialize WiFi
   if(initWiFi())
	   printf("WiFi OK!\n");
   else
	   printf("Error WiFi\n");

   //Open Socket
   if(openSocket())
   {
	   printf("Socket OK!\n");
   	   u32 ip = Wifi_GetIP();
   	   iprintf("\nIP: [%li.%li.%li.%li]\n", (ip ) & 0xFF, (ip >> 8) & 0xFF, (ip >> 16) & 0xFF, (ip >> 24) & 0xFF);
   }
   else
   {
	   printf("Error Socket\n");
   }



    while(1){
    	//Send a message if key is pressed
		sendMessageTest();
		//Receive message of someone else that pressed a key
		receiveMessageTest();

        swiWaitForVBlank();
    }
}
