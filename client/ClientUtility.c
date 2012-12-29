////////////////////////////////////////////////////////////
/*
	File Name:		ClientUtility.c
	Instructor:		Prof. Arthur Goldberg
	Author:			Shen Li
	UID:			N14361265
	Department:		Computer Science
	Note:			This ClientUtility.c file includes 
					Setup Socket, Send Ping Message,
					and Receive Ping Message Functions.
*/
////////////////////////////////////////////////////////////

///////////////HEADER FILES///////////////
#include "Client.h"

///////////////FUNCTIONS///////////////
/*Setup Client Socket Function
  Variable Definition:
  -- host: socket host name or ip address
  -- service: socket service name or port number
  Return value: socket descriptor for client
*/
int setupClientSocket(const char *host, const char *service){
	struct addrinfo		address_criteria;		//criteria for address match
	struct addrinfo		*server_address;		//holder for returned list of server addresses
	int					return_value;			//return value

	//Tell the system what kind(s) of address info we want
	memset(&address_criteria, 0, sizeof(address_criteria));	//zero the address_criteria
	address_criteria.ai_family = AF_UNSPEC;					//any address family
	address_criteria.ai_socktype = SOCK_DGRAM;				//only datagram sockets
	address_criteria.ai_protocol = IPPROTO_UDP;				//only UDP protocol

	//Get address(es)
	return_value = getaddrinfo(host, service, &address_criteria, &server_address);
	//Success returns zero
	if (return_value != 0){
		dieWithUserMessage("getaddrinfo() failed", gai_strerror(return_value));
	}

	//Create socket for connecting the server
	for (address = server_address; address != NULL; address = address->ai_next){
		//Create a unreliable, datagram socket using UDP
		client_socket = socket(address->ai_family, address->ai_socktype, address->ai_protocol);
		//Socket creation failed
		if (client_socket < 0){
			continue;
		}
		else{
			break;
		}
		//Socket creation failed and close socket
		close(client_socket);
		client_socket = -1;
	}

	//Free addrinfo allocated in getaddrinfo()
	freeaddrinfo(server_address);

	return client_socket;
}

/*Send Ping Message Function
  Variable Definition:
  -- NULL
  Return value: NULL
*/
void sendPingMessage(){
	char		*buffer = (char*)malloc(sizeof(char) * BUFFER_SIZE);	//word to ping
	size_t		buffer_length;											//ping request message length
	ssize_t		number_bytes;											//size of send message

	//Construct Ping message: PING <n> <time> CRLF
	buffer = constructRequestMessage(send_count);
	//Set length of ping message
	buffer_length = strlen(buffer);
	//Test the length of ping message
	if (buffer_length > BUFFER_SIZE){
		dieWithUserMessage(buffer, "Too long!");
	}
	//Send the ping request message to the server
	number_bytes = sendto(	client_socket,
							buffer,
							buffer_length,
							MSG_NOSIGNAL,
							address->ai_addr,
							address->ai_addrlen);
	//Test the sending is successful
	if (number_bytes < 0){
		dieWithSystemMessage("sendto() failed");
	}
	else if (number_bytes != buffer_length){
		dieWithUserMessage("sendto() error", "send unexpected number of bytes!");
	}
	//Increase the send_count number
	send_count++;
	//Free buffer
	free(buffer);

	return;
}

/*Receive Ping Message Function
  Variable Definition:
  -- wait_time: time need to wait
  Return value: NULL
*/
void rcvdPingMessage(int wait_time){
	struct sockaddr_storage	from_address;				//from address structure
	struct timeval			rcvd_time;					//received time
	socklen_t				from_address_length;		//from address length
	char					buffer[BUFFER_SIZE + 1];	//I/O buffer
	ssize_t					number_bytes;				//size of received message

	//Initialize buffer
	memset(buffer, 0, BUFFER_SIZE + 1);
	//Set length of from address structure
	from_address_length = sizeof(from_address);
	//Set the timer
	setTimer(wait_time, ITIMER_REAL);
	//Receive a response
	if ((number_bytes = recvfrom(	client_socket,
									buffer,
									BUFFER_SIZE,
									0,
									(struct sockaddr*)&from_address,
									&from_address_length)) < 0){
		//Alarm went off
		if (errno == EINTR){
			//Request timed out
			switch (wait_time){
				case TIMEOUT:
					fputs("Request timed out!", stdout);
					fputc('\n', stdout);
					break;
				default:
					break;
			}
			return;
		}
		else{
			dieWithSystemMessage("recvfrom() failed");
		}
	}
	//Get the received time
	gettimeofday(&rcvd_time, NULL);
	//Test the receive is successful
	if (number_bytes != strlen(buffer)){
		dieWithUserMessage("recvfrom() error", "received unexpected number of bytes!");
	}
	//Verify reception from expected source
	if (!socketAddressEqual(address->ai_addr, (struct sockaddr*)&from_address)){
		dieWithUserMessage("recvfrom() error", "received a packet from unknown source");
	}
	//Construct response message
	constructResponseMessage(	buffer,
								(struct sockaddr*)&from_address,
								&rcvd_time);
	//Increase the rcvd_count number
	rcvd_count++;
	//Sleep in order to send exactly 1 ping per second
	isSleep();

	return;
}
