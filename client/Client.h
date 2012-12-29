////////////////////////////////////////////////////////////////
/*
	File Name:		Client.h
	Instructor:		Prof. Arthur Goldberg
	Author:			Shen Li
	UID:			N14361265
	Department:		Computer Science
	Note:			This Client.h file includes
					HEADER FILES, MACRO, STRUCT DEFINITION,
					GLOBAL VARIABLE AND FUNCTION DECLARATION.
*/
////////////////////////////////////////////////////////////////

///////////////PRECOMPILER///////////////
#ifndef	CLIENT_H_
#define CLIENT_H_

///////////////DEBUG///////////////
//#define DEBUG 1
#ifdef DEBUG
	#define DEBUG_PRINT printf("%s-%s:%d:", __FILE__, __FUNCTION__, __LINE__)
#else
	#define DEBUG_PRINT
#endif

///////////////HEADER FILES///////////////
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>
#include <signal.h>
#include <errno.h>

///////////////MACRO///////////////
#define	TIMEOUT			1
#define	REPLY_TIMEOUT	5
#define	CRLF			"\r\n"

///////////////STRUCT DEFINITION///////////////
/*RTT structure*/
typedef struct _rtt_info{
	int					req;	//request number
	double				rtt;	//rtt value
	struct _rtt_info	*next;	//next pointer
}RTT_INFO;

///////////////GLOBAL VARIABLE///////////////
enum	size_constants{			//size constants
	PING_SIZE		= 10,
	BUFFER_SIZE 	= 1024,
};
RTT_INFO	*header;			//_rtt_info structure header
RTT_INFO	*node;				//_rtt_info structure node
struct addrinfo	*address;		//addrinfo structure
int		client_socket;			//socket descriptor for client
int		send_count;				//sending ping message count
int		rcvd_count;				//received ping message count

///////////////FUNCTION DECLARATION///////////////
/*DieWithMessage.c*/
void 	dieWithUserMessage(const char *message, const char *detail);
void 	dieWithSystemMessage(const char *message);
/*ClientUtility.c*/
int		setupClientSocket(const char *host, const char *service);
void	sendPingMessage();
void	rcvdPingMessage(int wait_time);
/*AddressUtility.c*/
void 	printSocketAddress(const struct sockaddr *address, FILE *stream);
bool	socketAddressEqual(const struct sockaddr *address_1, const struct sockaddr *address_2);
/*HandleResponseUtility.c*/
void	catchAlarm(int ignored);
void	initPinger();
void	cleanPinger(RTT_INFO *node);
char*	constructRequestMessage(int number);
void	constructResponseMessage(	const char *response,
									const struct sockaddr *address,
									struct timeval *rcvdtime);
void	constructStatistics();
/*TimeUtility.c*/
void	setTimer(int timeout, int timer_type);
void	isSleep();
void	timevalSubstruct(struct timeval *out, struct timeval *in);
double	getMaxRTT();
double	getMinRTT();
double	getAvgRTT();
double	getMDevRTT();

#endif //CLIENT_H
