/*communication protocol.

all messages relevant to game. underlying protocol is UDP socket communication with
WIFI_minilib.c

example:

Master:                             | Slave:
---------------------------------------------------------
                                    | send JOIN
    send ack                        | 
    send_ships                      | 
                                    | send_ships                       | 
game loop ---------------------------------------------------------
    send_shot                       | 
                                    | send ack
                                    | send_shot 

*/
#ifndef MESSAGE_PROTOCOL_H
#define MESSAGE_PROTOCOL_H

#include "WiFi_minilib.h"
#include "stdlib.h"
#include "battleships.h"
typedef enum{JOIN=1, SHIPS=2, SHOT=3, ACK=4} message_type;


#define MSG_SIZE 25

#define HEADER_LEN 1
#define JOIN_MSG_LEN HEADER_LEN
#define SHIP_MSG_BODY (CARRIER_SIZE + BATTLESHIP_SIZE + CRUISER_SIZE + SUBMARINE_SIZE + DESTROYER_SIZE)
#define SHOT_MSG_BODY 1

extern char send_buffer[MSG_SIZE];
extern char recv_buffer[MSG_SIZE];

// Initializes wifi and opens socket for communication, with debug printing

void wifi_test();

// Initializes wifi and opens socket for communication
// Access point SSID is defined in WIFI_Minilib.h

void wifi_init();

/*
    sends message of a specified type
    set body to NULL if no body. 
    body can point to ships or shot data.

    blocks until message is acknowledged.

*/
void sendMessage(message_type type, char* body);
/*
    receive message of a specified type.
    Blocks until it reads.
*/
int recvMessage(message_type type);

#endif
