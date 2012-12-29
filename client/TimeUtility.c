/////////////////////////////////////////////////////////////////
/*
	File Name:		TimeUtility.c
	Instructor:		Prof. Arthur Goldberg
	Author:			Shen Li
	UID:			N14361265
	Department:		Computer Science
	Note:			This TimeUtility.c file includes Timer, 
					Calculate and Statistic RTT Functions.
*/
/////////////////////////////////////////////////////////////////

///////////////HEADER FILES///////////////
#include "Client.h"

///////////////FUNCTIONS///////////////
/*Set Timer Function
  Variable Definition:
  -- timeout: time out value
  -- timer_type: timer type (ITIMER_REAL, ITIMER_VIRTUAL, ITIMER_PROF)
  Return value: NULL
*/
void setTimer(int timeout, int timer_type){
	struct itimerval	timer;		//timer structure

	//Set the time out value
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = 0;
	//Set the first time out value
	timer.it_value.tv_sec = timeout;
	timer.it_value.tv_usec = 0;

	//Set the timer
	if (setitimer(timer_type, &timer, NULL) != 0){
		dieWithSystemMessage("setitimer() failed");
	}

	return;
}

/*Process Sleeping Function
  Variable Definition:
  -- NULL
  Return value: NULL
*/
void isSleep(){
	struct itimerval	timer;			//timer structure
	useconds_t			sleep_time;		//process sleep time (microseconds)

	//Get the current SIGALRM timer
	if (getitimer(ITIMER_REAL, &timer) != 0){
		dieWithSystemMessage("getitimer() failed");
	}
	//Calculate the microseconds that the process need to sleep
	sleep_time = timer.it_value.tv_sec * 1000000 + timer.it_value.tv_usec;
	//Start sleeping
	usleep(sleep_time);

	return;
}

/*Calculate the Difference between Send and Receive Time Function
  Variable Definition:
  -- out: pointer to the received time
  -- in: pointer to the send time
  Return value: NULL
*/
void timevalSubstruct(struct timeval *out, struct timeval *in){
	//Calculate the microseconds
	if ((out->tv_usec -= in->tv_usec) < 0){
		--out->tv_sec;
		out->tv_usec += 1000000;
	}
	//Calculate the seconds
	out->tv_sec -= in->tv_sec;

	return;
}

/*Get Max RTT Value Function
  Variable Definition:
  -- NULL
  Return value: the maximum rtt value
*/
double getMaxRTT(){
	RTT_INFO	*p;			//pointer to _rtt_info structure
	double		max_rtt;	//max rtt value

	//Initialize first node of _rtt_info link
	p = header->next;
	//Initialize max rtt value
	max_rtt = p->rtt;
	while (p != NULL){
		//Find the max rtt value
		if (max_rtt < p->rtt){
			max_rtt = p->rtt;
		}
		p = p->next;
	}

	return max_rtt;
}

/*Get Min RTT Value Function
  Variable Definition:
  -- NULL
  Return value: the minimum rtt value
*/
double getMinRTT(){
	RTT_INFO	*p;			//pointer to _rtt_info structure
	double		min_rtt;	//min rtt value

	//Initialize first node of _rtt_info link
	p = header->next;
	//Initialize min rtt value
	min_rtt = p->rtt;
	while (p != NULL){
		//Find the min rtt value
		if (min_rtt > p->rtt){
			min_rtt = p->rtt;
		}
		p = p->next;
	}

	return min_rtt;
}

/*Get Average RTT Value Function
  Variable Definition:
  -- NULL
  Return value: the average rtt value
*/
double getAvgRTT(){
	RTT_INFO	*p;				//pointer to _rtt_info structure
	double		avg_rtt = 0;	//average rtt value

	//Initialize first node of _rtt_info link
	p = header->next;
	while (p != NULL){
		//Get the sum of all rtt value
		avg_rtt += p->rtt;
		p = p->next;
	}
	//Get the average rtt value
	avg_rtt /= rcvd_count;

	return avg_rtt;
}

/*Get Variance RTT Value Function
  Variable Definition:
  -- NULL
  Return value: the variance rtt value
*/
double getMDevRTT(){
	RTT_INFO	*p;						//pointer to _rtt_info strucuture
	double		avg_rtt = getAvgRTT();	//average rtt value
	double		mdev_rtt = 0;			//variance rtt value

	//Initialize first node of _rtt_info link
	p = header->next;
	while (p != NULL){
		//Get the sum
		mdev_rtt += (p->rtt - avg_rtt) * (p->rtt - avg_rtt);
		p = p->next;
	}
	//Get the variance
	mdev_rtt /= rcvd_count;

	return mdev_rtt;
}
