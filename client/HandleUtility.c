////////////////////////////////////////////////////////////////////
/*
	File Name:		HandleUtility.c
	Instructor:		Prof. Arthur Goldberg
	Author:			Shen Li
	UID:			N14361265
	Department:		Computer Science
	Note:			This HandleUtility.c file includes 
					Handle SIGALRM signal, Initialize UDPPinger,
					Construct Ping Request Message, Construct
					Ping Response Message, and Construct Ping
					Statistics Functions.
*/
////////////////////////////////////////////////////////////////////

///////////////HEADER FILES///////////////
#include "Client.h"

///////////////FUNCTIONS///////////////
/*Handle for signal Function
  Variable Definition:
  -- ignored: signal type
  Return value: NULL
*/
void catchAlarm(int ignored){
	//Acroding to the signal type
	switch (ignored){
		//Interrupt signal
		case SIGINT:
			//Construct Ping statistics
			constructStatistics();
			//Close socket
			close(client_socket);
			//Clean allocate memory
			cleanPinger(header);
			//Exit the UDPPinger
			exit(0);
			break;
		//Alarm signal
		case SIGALRM:
			break;
		default:
			break;
	}

	return;
}

/*Initialize Global Variables Function
  Variable Definition:
  -- NULL
  Return value: NULL
*/
void initPinger(){
	//Initialize client socket
	client_socket = -1;
	//Initialize sending count
	send_count = 0;
	//Initialize received count
	rcvd_count = 0;
	//Initialize _rtt_info structure header
	header = (RTT_INFO*)malloc(sizeof(RTT_INFO));
	header->req = 0;
	header->rtt = rcvd_count;
	header->next = NULL;
	//Initialize _rtt_info structure node
	node = header;

	return;
}

/*Clean Global Variables Function
  Variable Definition:
  -- node: _rtt_info structure node
  Return value: NULL
*/
void cleanPinger(RTT_INFO *node){
	//Free _rtt_info structure
	if (node->next != NULL){
		cleanPinger(node->next);
	}
	free(node);
}

/*Construct Ping Request Message Function
  Variable Definition:
  -- number: the number of ping request message
  Return value: ping request message string
*/
char* constructRequestMessage(int number){
	struct timeval	*send_time = (struct timeval*)malloc(sizeof(struct timeval));	//sending time
	char			*ping_request = (char*)malloc(sizeof(char) * BUFFER_SIZE);		//ping request message

	//Get the sending time
	gettimeofday(send_time, NULL);
	//Construct ping message
	sprintf(ping_request, "PING %d %u %u %s", number, (unsigned)send_time->tv_sec, (unsigned)send_time->tv_usec, CRLF);
	//Free timeval structure
	free(send_time);

	return ping_request;
}

/*Construct Ping Response Message Function
  Variable Definition:
  -- response: received buffer data
  -- address: received address information
  -- rtime: received time (seconds and microseconds)
  Return value: NULL
*/
void constructResponseMessage(	const char *response,
								const struct sockaddr *address,
								struct timeval *rtime){
	struct timeval	*stime = (struct timeval*)malloc(sizeof(struct timeval));	//sending time structure
	int				req_number;													//UDP request number
	unsigned		sec_time;													//sending time second(s)
	unsigned		usec_time;													//sending time microsecond(s)

	//Get the req number, sending second(s), and sending microsecond(s) time
	if (sscanf(response, "%d %u %u", &req_number, &sec_time, &usec_time) != 3){
		fputs(response, stdout);
		fputs(": Received data error!", stdout);
		fputc('\n', stdout);
		return;
	}
	//Assign the variables of struct timeval
	stime->tv_sec = sec_time;
	stime->tv_usec = usec_time;
	//Calculate the difference between sending time and received time
	timevalSubstruct(rtime, stime);
	//Allocate the memory for new node
	node->next = (RTT_INFO*)malloc(sizeof(RTT_INFO));
	//Let the node pointer point to the current _rtt_info structure
	node = node->next;
	//Assign the variables (including request number and rtt)
	node->req = req_number;
	node->rtt = rtime->tv_sec * 1000.0 + rtime->tv_usec / 1000.0;
	node->next = NULL;
	//Output the ping result information
	fputs("Reply from ", stdout);
	printSocketAddress(address, stdout);
	printf(": udp_req = %d, rtt = %.3f ms %s", req_number, node->rtt, CRLF);
	//Free timeval structure
	free(stime);

	return;
}

/*Construct Ping Statistics Function
  Variable Definition:
  -- NULL
  Return value: NULL
*/
void constructStatistics(){
	//Output
	fputc('\n', stdout);
	fputs("--- ping statistics ---", stdout);
	fputc('\n', stdout);
	//Calculate the result
	printf("%d packet(s) transmitted, %d received, %.2f%% packet loss\n",	send_count,
																			rcvd_count,
																			(double)(send_count - rcvd_count) / send_count * 100);
	//Calculate the rtt statistics
	if (rcvd_count != 0){
		printf("rtt max/min/avg/mdev = %.3f/%.3f/%.3f/%.3f ms\n",	getMaxRTT(),
																	getMinRTT(),
																	getAvgRTT(),
																	getMDevRTT());
	}

	return;
}
