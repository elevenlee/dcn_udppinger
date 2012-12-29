/////////////////////////////////////////////////////////
/*
	File Name:		ServerUtility.c
	Instructor:		Prof. Arthur Goldberg
	Author:			Shen Li
	UID:			N14361265
	Department:		Computer Science
	Note:			This ServerUtility.c file includes 
					Setup UDP Server Socket, Use Server 
					Idle Time, and Send-Receive Function.
*/
/////////////////////////////////////////////////////////

///////////////HEADER FILES///////////////
#include "Server.h"

///////////////FUNCTIONS///////////////
/*Setup Server Socket Function
  Variable Definition:
  -- service: socket service name or port number
  Return value: server socket number
*/
int setupServerSocket(const char *service){
	struct addrinfo 		address_criteria;	//criteria for address match
	struct addrinfo 		*server_address;	//list of server addresses
	struct addrinfo 		*address;			//pointer to addresses node
	struct sockaddr_storage local_address;		//print local address
	socklen_t 				address_size;		//address size
	int 					return_value;		//return value
	
	//Construct the server address structure
	memset(&address_criteria, 0, sizeof(address_criteria));	//zero the address_criteria
	address_criteria.ai_family = AF_UNSPEC;					//any address family
	address_criteria.ai_flags = AI_PASSIVE;					//accept on any address/port
	address_criteria.ai_socktype = SOCK_DGRAM;				//only datagram sockets
	address_criteria.ai_protocol = IPPROTO_UDP;				//only udp protocol
	
	//Get address/name information
	return_value = getaddrinfo(NULL, service, &address_criteria, &server_address);
	//Success returns zero
	if (return_value != 0){
		dieWithUserMessage("getaddrinfo() failed!", gai_strerror(return_value));
	}
	
	//Create socket for incoming connections
	for (address = server_address; address != NULL; address = address->ai_next){
		//Initialize the server socket
		server_socket = -1;
		//Create socket for incoming connections
		server_socket = socket(server_address->ai_family, server_address->ai_socktype, server_address->ai_protocol);
		//if socket creation failed, try next address in the list
		if (server_socket < 0){
			continue;
		}
		
		//Bind to the server local address and set socket to the list
		if (bind(server_socket, server_address->ai_addr, server_address->ai_addrlen) == 0){
			//Get address size
			address_size = sizeof(local_address);
			//Get socket name
			if (getsockname(server_socket, (struct sockaddr*)&local_address, &address_size) < 0){
				dieWithSystemMessage("getsockname() failed!");
			}
			//Output local address and port of socket(listening address and port)
			fputs("Binding to ", stdout);
			printSocketAddress((struct sockaddr*)&local_address, stdout);
			fputc('\n', stdout);
			//Bind and list successful
			break;
		}
		//Close and try again
		close(server_socket);
	}
	//Free address list allocated by getaddrinfo()
	freeaddrinfo(server_address);
	
	return server_socket;
}

/*Execution During Idle Time Function
  Variable Definition:
  -- NULL
  Return value: NULL
*/
void useIdleTime(){
	//Output
	fputs("<This is UDPServer idle time!>", stdout);
	fputc('\n', stdout);
	//3 seconds of activity
	sleep(5);

	return;
}

/*Handle SIGIO Function
  Variable Definition:
  -- signal_type: signal type variable
  Return value: NULL
*/
void SIGIOHandler(int signal_type){
	char		buffer[BUFFER_SIZE];	//datagram buffer
	size_t		buffer_length;			//datagram buffer length
	ssize_t		number_bytes_rcvd;		//size of received message
	ssize_t		number_bytes_send;		//size of send message

	//As long as there is input...
	do{
		struct sockaddr_storage	client_address;				//client address
		socklen_t				client_address_length;		//length of client address structure

		//Set length of client address structure
		client_address_length = sizeof(client_address);
		//Initialize request buffer
		memset(buffer, 0, BUFFER_SIZE);
		//Block until receive message from a client
		number_bytes_rcvd = recvfrom(	server_socket,
										buffer,
										BUFFER_SIZE,
										0,
										(struct sockaddr*)&client_address,
										&client_address_length);
		//Receive is failed
		if (number_bytes_rcvd < 0){
			//Only acceptable error: recvfrom() would have blocked
			if (errno != EWOULDBLOCK){
				dieWithSystemMessage("rcvdfrom() failed!");
			}
		}
		//Receivei is successful
		else{
			//Now, server has received client message
			//Output the client address and port
			fputs("Handling client ", stdout);
			printSocketAddress((struct sockaddr*)&client_address, stdout);
			fputc('\n', stdout);

			//Handle Client UDP Ping Request
			handleClientRequest(buffer);
			//Set UDP request length
			buffer_length = strlen(buffer);

			//Get a random number in order to simulate the packet loss
			if (randomNumber() > LOST_RATE){
				//Send received datagram back to the client
				number_bytes_send = sendto(	server_socket,
											buffer,
											buffer_length,
											MSG_NOSIGNAL,
											(struct sockaddr*)&client_address,
											sizeof(client_address));
				//Test the send is successful
				if (number_bytes_send < 0){
					dieWithSystemMessage("sendto() failed!");
				}
				else if (number_bytes_send != buffer_length){
					dieWithUserMessage("sendto()", "sent unexpected number of bytes");
				}
			}
			//Packet that send to the client is lost
			else{
				fputs("Packet that send to the client is LOST!", stdout);
				fputc('\n', stdout);
			}
		}
	}while (number_bytes_rcvd >= 0);
	//Nothing left to receive
	return;
}
