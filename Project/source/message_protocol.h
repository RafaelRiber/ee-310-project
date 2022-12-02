/*communication protocol.
note: 
- WIFI_monilib are non-blocking. means that it will not wait for data to be present. 
- Game logic expects always expects data. We will have to loop receiveData until it reads
- confirm messages by reading the header. It should be what it expects. 
    - Eg Expecting a send_shot message
- once message is confirmed send ACK.

- error handling: 
    - If no ack within X time. something is very wrong. 
    - simple solution: restart game 
    - complicated: attempt to recover connection?


we have a "server" that relays all messages but it won't do any logic.
its simply a relay. So, we will use one device as a "Master" of the game, and
the other as a "Slave" to establish the game. 


Messages:
request_join
    -msg_id = 0 
send_ships
    -msg_id = 1
send_shot
    -msg_id = 2
ack
    -msg_id = 3

example:

Master:                             | Slave:
---------------------------------------------------------
                                    | send request_join
    send ack                        | 
    send_ships                      | 
                                    | send ack
                                    | send_ships
    send ack                        | 
game loop ---------------------------------------------------------
    send_shot                       | 
                                    | send ack
                                    | send_shot 

- each nintendo indepdnantly keeps state of game. They simply tell eachother the shot.
- this will make game logic easier. there will be less difference between the Master and slave.
- It also means we do not have to communicate to end the game. It will be done independalty. 
*/
#ifndef MESSAGE_PROTOCOL_H
#define MESSAGE_PROTOCOL_H

#include "WiFi_minilib.h"
#include "stdlib.h"
#include "battleships.h"
typedef enum{JOIN, SHIPS, SHOT, ACK} message_type;



#define HEADER_LEN 1

#define JOIN_MSG_LEN HEADER_LEN 
#define SHIP_MSG_LEN HEADER_LEN + CARRIER_SIZE +BATTLESHIP_SIZE + CRUISER_SIZE + SUBMARINE_SIZE + DESTROYER_SIZE
#define SHOT_MSG_LEN HEADER_LEN + 1
#define ACK_MSG_LEN HEADER_LEN

char send_buffer[SHIP_MSG_LEN];
char recv_buffer[SHIP_MSG_LEN];

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

#endif