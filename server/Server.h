////////////////////////////////////////////////////////////////
/*
	File Name:		Server.h
	Instructor:		Prof. Arthur Goldberg
	Author:			Shen Li
	UID:			N14361265
	Department:		Computer Science
	Note:			This Server.h file includes
					HEADER FILES, MACRO, STRUCT DEFINITION,
					GLOBAL VARIABLE AND FUNCTION DECLARATION.
*/
////////////////////////////////////////////////////////////////

///////////////PRECOMPILER///////////////
#ifndef	SERVER_H_
#define SERVER_H_

///////////////DEBUG///////////////
//#define DEBUG 1
#ifdef DEBUG
	#define DEBUG_PRINT printf("%s-%s:%d:", __FILE__, __FUNCTION__, __LINE__)
	#define	LOST_RATE	0
#else
	#define DEBUG_PRINT
	#define	LOST_RATE	0.4
#endif

///////////////HEADER FILES///////////////
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <regex.h>

///////////////MACRO///////////////
#define	CRLF			"\r\n"

///////////////STRUCT DEFINITION///////////////

///////////////GLOBAL VARIABLE///////////////
enum	size_constants{		//size constatns
	ONE_SIZE		= 1,
	NAME_SIZE 		= 16,
	STRING_SIZE 	= 128,
	BUFFER_SIZE 	= 1024,
};
int		server_socket;		//socket descriptor for server

///////////////FUNCTION DECLARATION///////////////
/*DieWithMessage.c*/
void 	dieWithUserMessage(const char *message, const char *detail);
void 	dieWithSystemMessage(const char *message);
/*ServerUtility.c*/
int 	setupServerSocket(const char *service);
void	useIdleTime();
void	SIGIOHandler(int signal_type);
/*AddressUtility.c*/
void 	printSocketAddress(const struct sockaddr *address, FILE *stream);
/*HandleRequestUtility.c*/
double	randomNumber();
void	handleClientRequest(char *request);
bool	syntaxChecking(const char *string, const char signal_value);

#endif //SERVER_H
