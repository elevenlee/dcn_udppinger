////////////////////////////////////////////////////////////
/*
	File Name:		AddressUtility.c
	Instructor:		Prof. Arthur Goldberg
	Author:			Shen Li
	UID:			N14361265
	Department:		Computer Science
	Note:			This AddressUtility.c file includes 
					Handle Socket Address Functions.
*/
////////////////////////////////////////////////////////////

///////////////HEADER FILES///////////////
#include "Client.h"

///////////////FUNCTIONS///////////////
/*Print Socket Address Function
  Variable Definition:
  -- address: socket address structure
  -- stream: file stream(stdin, stdout, stderr, or file objective...)
  Return Value: NULL
*/
void printSocketAddress(const struct sockaddr *address, FILE *stream){
	void 		*numeric_address;					//pointer to binary address
	char 		address_buffer[INET6_ADDRSTRLEN];	//address data
	in_port_t 	port;								//port number
	
	//Test for address and stream
	if (address == NULL || stream == NULL){
		return;
	}
	
	//Set pointer to address based on address family
	switch (address->sa_family){
		//The type of address is ipv4
		case AF_INET:
			numeric_address = &((struct sockaddr_in*)address)->sin_addr;
			port = ntohs(((struct sockaddr_in*)address)->sin_port);
			break;
		//The type of address is ipv6
		case AF_INET6:
			numeric_address = &((struct sockaddr_in6*)address)->sin6_addr;
			port = ntohs(((struct sockaddr_in6*)address)->sin6_port);
			break;
		//Invalid address
		default:
			fputs("[unknown type]", stream);
			return;
	}
	
	//Convert binary to printable address
	if (inet_ntop(address->sa_family, numeric_address, address_buffer, sizeof(address_buffer)) == NULL){
		//Unable to convert
		fputs("[invalid address]", stream);
	}
	else{
		//Output the address
		fprintf(stream, "%s", address_buffer);
		if (port != 0){
			//Output the port number
			fprintf(stream, "-%u", port);
		}
	}

	return;
}

/*Test Two Addresses Are Equal Function
  Variable Definition:
  -- address_1: socket address structure No.1
  -- address_2: socket address structure No.2
  Return Value: If two addresses are equal, return true; else return false
*/
bool socketAddressEqual(const struct sockaddr *address_1, const struct sockaddr *address_2){
	struct sockaddr_in	*ipv4_address_1;		//ipv4 address structure No.1
	struct sockaddr_in	*ipv4_address_2;		//ipv4 address structure No.2
	struct sockaddr_in6	*ipv6_address_1;		//ipv6 address structure No.1
	struct sockaddr_in6	*ipv6_address_2;		//ipv6 address structure No.2

	//Test address No.1 or address No.2 is empty
	if (address_1 == NULL || address_2 == NULL){
		return address_1 == address_2;
	}
	//IPv4 compare
	else if (address_1->sa_family == AF_INET){
		//Initialize sockaddr_in structure
		ipv4_address_1 = (struct sockaddr_in*)address_1;
		ipv4_address_2 = (struct sockaddr_in*)address_2;
		//Compare address and port number
		return	ipv4_address_1->sin_addr.s_addr == ipv4_address_2->sin_addr.s_addr
				&& ipv4_address_1->sin_port == ipv4_address_2->sin_port;
	}
	//IPv6 compare
	else if (address_1->sa_family == AF_INET6){
		//Initialize sockaddr_in6 structure
		ipv6_address_1 = (struct sockaddr_in6*)address_1;
		ipv6_address_2 = (struct sockaddr_in6*)address_2;
		//Compare address and port number
		return	memcmp(&ipv6_address_1->sin6_addr, &ipv6_address_2->sin6_addr, sizeof(struct in6_addr)) == 0
				&& ipv6_address_1->sin6_port == ipv6_address_2->sin6_port;
	}
	else{
		return false;
	}
}
