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
#include "Server.h"

///////////////FUNCTIONS///////////////
/*Main Function
  Variable Definition:
  -- argc: the number of command arguments
  -- argv[]: each vairable of command arguments(argv[0] is the path of execution file forever)
  Return Value: Server exit number
*/
int main(int argc, char *argv[]){
	//Test for correct number of arguments
	if (argc != 2){
		dieWithUserMessage("Parameter(s)", "<Server Port/Service>");
	}

	struct sigaction	handler;				//sigaction structure
	char				*service = argv[1];		//first argument: server listening port number

	//Create socket for incoming connections
	server_socket = setupServerSocket(service);
	//server_socket = setupServerSocket(service);
	if (server_socket < 0){
		dieWithSystemMessage("setupServerSocket() failed!");
	}

	//Set signal handler for SIGIOHandler
	handler.sa_handler = SIGIOHandler;
	//Create mask that mask all signals
	if (sigfillset(&handler.sa_mask) < 0){
		dieWithSystemMessage("sigfillset() failed!");
	}
	//No flags
	handler.sa_flags = 0;
	//Set the "SIGIO" signal
	if (sigaction(SIGIO, &handler, 0) < 0){
		dieWithSystemMessage("sigaction() failed for SIGIO!");
	}
	//We must own the socket to receive the SIGIO message
	if (fcntl(server_socket, F_SETOWN, getpid()) < 0){
		dieWithSystemMessage("Unable to set process owner to us!");
	}
	//Arrage for nonblocking I/O and SIGIO delivery
	if (fcntl(server_socket, F_SETFL, O_NONBLOCK | FASYNC) < 0){
		dieWithSystemMessage("Unable to put client sock into non-blocking/async mode!");
	}

	//Go off and do real work; Echoing happens in the background
	for(;;){
		useIdleTime();
	}
	
	return 0;
}
