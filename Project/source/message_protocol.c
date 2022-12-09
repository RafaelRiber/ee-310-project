#include "message_protocol.h"

 char send_buffer[SHIP_MSG_LEN];
 char recv_buffer[SHIP_MSG_LEN];

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
int sendMessage(message_type type, unsigned char* body) {
    int bytes;
    send_buffer[0] = type;
    switch(type) {
        case JOIN:
            bytes = JOIN_MSG_LEN;
        break;
        case SHIPS:
            bytes = SHIP_MSG_LEN;
            memcpy(send_buffer+1, body, bytes-1);
        break;
        case SHOT:
            bytes = SHOT_MSG_LEN;
            memcpy(send_buffer+1, body, bytes-1);
        break;
        case ACK:
            bytes = ACK_MSG_LEN;
        break;
    }

    sendData(send_buffer, bytes);
    if (type != ACK) {
        while(recv_buffer[0] != ACK)
            receiveData(recv_buffer, ACK_MSG_LEN);
    }

    return 0;
}
/*
    receive message of a specified type.
    Blocks until it reads.
*/
int recvMessage(message_type type) {
    int bytes = 1;
    switch(type) {
        case JOIN:
            bytes = JOIN_MSG_LEN;
        break;
        case SHIPS:
            bytes = SHIP_MSG_LEN;
        break;
        case SHOT:
            bytes = SHOT_MSG_LEN;
        break;
        case ACK:
            bytes = ACK_MSG_LEN;
        break;
    }

    while(recv_buffer[0] != type)
        receiveData(recv_buffer, bytes);

    return 0;
}
