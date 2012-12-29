///////////////////////////////////////////////
/*
	File Name:		MainFunction.c
	Instructor:		Prof. Arthur Goldberg
	Author:			Shen Li
	UID:			N14361265
	Department:		Computer Science
	Note:			This MainFunction.c file
					includes Main Function.
*/
///////////////////////////////////////////////

///////////////HEADER FILES///////////////
#include "Client.h"

///////////////FUNCTIONS///////////////
/*Main Function
  Variable Definition:
  -- argc: the number of command arguments
  -- argv[]: each vairable of command arguments(argv[0] is the path of execution file forever)
  Return Value: Client exit number
*/
int main(int argc, char *argv[]){
	//Test for correct number of arguments
	if (argc != 3){
		dieWithUserMessage("Parameter(s)", "<Server Address/Name> <Server Port/Service>");
	}

	struct sigaction	handler;								//signal handler
	const char			*host = argv[1];						//first argument: host name/ip address
	const char			*service = argv[2];						//second argument: server listening port number
	int					rcvd_buffer_size = 50 * BUFFER_SIZE;	//received buffer size

	//Initialize Pinger
	initPinger();
	//Create a unreliable UDP socket
	client_socket = setupClientSocket(host, service);
	if (client_socket < 0){
		dieWithSystemMessage("setupClientSocket() failed");
	}
	//Set the received buffer size
	setsockopt(client_socket, SOL_SOCKET, SO_RCVBUF, &rcvd_buffer_size, sizeof(rcvd_buffer_size));
	//Set signal handler for alarm signal
	handler.sa_handler = catchAlarm;
	//Blocking everything in handler
	if (sigfillset(&handler.sa_mask) < 0){
		dieWithSystemMessage("sigfillset() failed");
	}
	//No flags
	handler.sa_flags = 0;
	//Set the "SIGALRM" signal
	if (sigaction(SIGALRM, &handler, 0) < 0){
		dieWithSystemMessage("sigaction() failed for SIGALRM");
	}
	//Set the "SIGINT" signal
	if (sigaction(SIGINT, &handler, 0) < 0){
		dieWithSystemMessage("sigaction() failed for SIGINT");
	}

	//Output the title
	printf("PING %s (", host);
	printSocketAddress((struct sockaddr*)address->ai_addr, stdout);
	printf(") result:\n");

	//Sleep for 1 second
	sleep(TIMEOUT);
	//Start to send ping message
	while (send_count < PING_SIZE){
		sendPingMessage();
		rcvdPingMessage(TIMEOUT);
	}
	
    
    //You should put sleep function inside the while loop and after rcvdPingMessage, and you should sleep for TIMEOUT - rtt value for this cycle #13 -4
    
    
    //Determine that no more replies that comes in
	if (send_count != rcvd_count){
		rcvdPingMessage(REPLY_TIMEOUT);
	}
	//SIGINT signal: construct ping statistics
	catchAlarm(SIGINT);
	
	return 0;
}
