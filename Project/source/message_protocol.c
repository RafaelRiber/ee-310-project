#include "message_protocol.h"

 char send_buffer[MSG_SIZE];
 char recv_buffer[MSG_SIZE];

// Initializes wifi and opens socket for communication, with debug printing

void wifi_test() {
	//Initialize WiFi
	if (initWiFi())
		printf("WiFi OK!\n");
	else
		printf("Error WiFi\n");

	//Open Socket
	if (openSocket()) {
		printf("Socket OK!\n");
		u32 ip = Wifi_GetIP();
		iprintf("\nIP: [%li.%li.%li.%li]\n", (ip) & 0xFF, (ip >> 8) & 0xFF,
				(ip >> 16) & 0xFF, (ip >> 24) & 0xFF);
	} else {
		printf("Error Socket\n");
	}
}

// Initializes wifi and opens socket for communication
// Access point SSID is defined in WIFI_Minilib.h

void wifi_init() {
	//Initialize WiFi
	initWiFi();

	//Open Socket
	openSocket();
}

/*
    sends message of a specified type
    set body to NULL if no body.
    body can point to ships or shot data.

    blocks until message is acknowledged.

*/
void sendMessage(message_type type, unsigned char* body) {
    send_buffer[0] = type;
    switch(type) {
       
        case SHIPS:
            memcpy(send_buffer+1, body, SHIP_MSG_BODY);
        break;
        case SHOT:
            memcpy(send_buffer+1, body, SHOT_MSG_BODY);
        break;
        default:
            //pass
        break;
    }

    sendData(send_buffer, MSG_SIZE);
    
}
/*
    receive message of a specified type.
    Blocks until it reads.
*/
int recvMessage(message_type type) {
    
    if (receiveData(recv_buffer, MSG_SIZE) > 0 && recv_buffer[0] == type)
        return 1;

    return 0;
}
