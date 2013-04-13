#include <stdio.h>

#include "uwmodem.h"
#include "frame.h"
#include "comm.h"

int init(unsigned char devid, unsigned char opmode){
/* Initialize queues for all nodes */
	initComms("COM7",300);

}

/**
 * put - this function sends the given string
 * internal use only
 * @param[out] x the modified input
 * @param[out] x the modified input
 * @return \f\$x + 1\f\$
 */
int put( int mode,
		 unsigned char devid,
		 char* fmt,
		 va_list args
		){
	
	/* Sanity checks */
	if(fmt == NULL)
		return 0;

	int returncode = 0;
	
	
	
	returncode = vsprintf(buffer, fmt, args);
	
	#ifdef DEBUG
		printf("uwmodem.c: put() -> user wants to send string:'%s'\n", buffer);
	#endif
	
	frameString(mode, devid, buffer);
	
	return returncode;
}

int get ( unsigned char devid,
		  char* fmt,
		  va_list args
		){
	
	int returncode = 0;
	
	/* Sanity checks */
	if(fmt == NULL)
		return 0;
	
	
	#ifdef DEBUG
		printf("uwmodem.c: get() -> user wants to get string in the following format: '%s'\n", fmt);
	#endif
	
	// note: IF BLOCKING, wait until we get something
	// 
	frames2s(devid, buffer);
	// ELSE
	/*
	// if we don't have anything, return an empty string
	//BLOCKING
	if(getSize(headRXF, tailRXF) ==0){
		buffer= "\0";
		return 0;
	}
	
	getString(devid, buffer);
	*/
	
	
	
	return vsscanf(buffer, fmt, args);
	
	
	
}

/**
 * sendString - this function sends the given string according to the given input.
 * @param[out] x the modified input
 * @param[out] x the modified input
 * @return \f\$x + 1\f\$
 */
int sendString ( unsigned char devid,
				 char* fmt,
				 ...
				){

	/* Variables used in this function */		
	int returncode = 0;
	va_list args;
	
	va_start(args, fmt);
	returncode = put(NORMAL_PRIORITY, devid, fmt, args);
	va_end(args);
	
	return returncode;

	}
	
int sendhi ( unsigned char devid,
			char* fmt,
			...
			){
	
	int returncode = 0;
	va_list args;
	
	va_start(args, fmt);
	returncode = put(HIGH_PRIORITY, devid, fmt, args);
	va_end(args);
	
	return returncode;

}
	
int sendna ( unsigned char devid,
			char* fmt,
			...
			){
	
	int returncode = 0;
	va_list args;
	
	va_start(args, fmt);
	returncode = put(NO_ACK, devid, fmt, args);
	va_end(args);
	
	return returncode;

}
	
int recvString ( char* fmt,
				 ...
				){
		  
	int returncode = 0;
	va_list args;
	
	va_start(args, fmt);
	returncode = get(SELF_ID, fmt, args);
	va_end(args);
	
	return returncode;
	
}

void sbroadcast( char* fmt, ... ){

	
	va_list args;
	
	va_start(args, fmt);
	put(NO_ACK, 0x00, fmt, args);
	va_end(args);
	

}

void rbroadcast( char* buffer, char* fmt, ... ){
	
	
	va_list args;
	
	va_start(args, fmt);
	get(0x00, fmt, args);
	va_end(args);
	
}
