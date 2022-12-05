#ifndef WIFI_MINILIB_H
#define WIFI_MINILIB_H

#include <nds.h>
#include <dswifi9.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define SSID "azy1"


//WiFi initialization
int initWiFi();
//Socket initialization
int openSocket();
//Receive data trough the socket
int receiveData(char* data_buff, int bytes);
//Send data through the socket
int sendData(char* data_buff, int bytes);
//Close the Socket
void closeSocket();
//Disconnect the WiFi
void disconnectFromWiFi();

////////////////////// TEST FUNCTIONS

typedef enum Messages_e {
	A,	//0x00
	B,	//0x01
	X,	//0x02
	Y	//0x03
}Message;

void sendMessageTest();
void receiveMessageTest();




#endif // WIFI_MINILIB_H
