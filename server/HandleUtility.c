/////////////////////////////////////////////////////////////////
/*
	File Name:		HandleUtility.c
	Instructor:		Prof. Arthur Goldberg
	Author:			Shen Li
	UID:			N14361265
	Department:		Computer Science
	Note:			This HandleUtility.c file includes 
					Handle Client UDP Ping Request, Generate
					Random Number, and Use RE Checking Functions.
*/
/////////////////////////////////////////////////////////////////

///////////////HEADER FILES///////////////
#include "Server.h"

///////////////FUNCTIONS///////////////
/*Handle Client UDP Ping Request Function
  Variable Definition:
  -- request: client UDP request message
  Return value: NULL
*/
void handleClientRequest(char *request){
	char		method[NAME_SIZE];			//method: PING
	int			seq_number;					//sequence number
	unsigned	sec_time;					//sending time (seconds)
	unsigned	usec_time;					//sending time (microseconds)

	//Initialize method, seq_number
	memset(method, 0, NAME_SIZE);

	//Test the Client UDP Request
	if (!syntaxChecking(request, 'P')){
		sprintf(request, "Ping Request is syntactically incorrect: PING <n> <time> CRLF%s", CRLF);
		return;
	}
	//Get the method, sequence number
	sscanf(request, "%s %d %u %u", method, &seq_number, &sec_time, &usec_time);
	//Reply UDP message (including request number and sending time)
	sprintf(request, "%d %u %u %s", seq_number + 1, sec_time, usec_time, CRLF);

	return;
}

/*Create a Random Number Function
  Variable Definition:
  -- NULL
  Return value: a random number between 0 and 1
*/
double randomNumber(){
	struct timeval	*random_time = (struct timeval*)malloc(sizeof(struct timeval));	//timeval structure
	double			ran_number;														//random number

	//Get the current time (including seconds and microseconds)
	gettimeofday(random_time, NULL);
	//Generate the random seed
	srand((unsigned)random_time->tv_usec);
	//Get a random number between 0 and 1
	ran_number = rand() / (double)(RAND_MAX);
	//Free timeval structure
	free(random_time);
	
	return ran_number;
}

/*Use Regular Expressions to check Request Message Function
  Variable Definition:
  -- string: string need to be checked
  -- signal_value: signal that decide the checking type
  Return Value: if matched, return 1; else return 0
*/
bool syntaxChecking(const char *string, const char signal_value){
	char 		*pattern = (char*)malloc(sizeof(char) * (BUFFER_SIZE));			//regular expression string
	char		*error_buffer = (char*)malloc(sizeof(char) * (STRING_SIZE));	//error buffer
	int 		status;															//result status
	size_t 		nmatch = ONE_SIZE;												//max number of match result
	regex_t 	reg;															//regex_t structure
	regmatch_t	pmatch[ONE_SIZE];												//match result
	
	//According to the signal value, determine using which regular expression
	switch(signal_value){
		//Ping Request line
		case 'P':
			pattern = "^PING[ ][0-9]+[ ][0-9]+[ ][0-9]+[ ]\r\n$";
			break;
		default:
			pattern = "";
			break;
	}
	//Compile the regular expression
	status = regcomp(&reg, pattern, REG_EXTENDED);
	if (status != 0){
		regerror(status, &reg, error_buffer, STRING_SIZE);
		dieWithUserMessage("regcomp() failed", error_buffer);
	}
	//Match the regular expression
	status = regexec(&reg, string, nmatch, pmatch, 0);
	//If there is no match result, return 0
	if (status == REG_NOMATCH){
		return false;
	}
	//Close the regex_t structure
	regfree(&reg);
	
	return true;
}
