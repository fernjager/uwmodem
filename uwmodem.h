#ifndef __UWMODEM_API__
#define __UWMODEM_API__

#include <stdarg.h>

#include "defines.h"


/**
 * send - this function sends the given string according to the given input.
 * @param[out] x the modified input
 * @param[out] x the modified input
 * @return The number of bytes succesfully transmitted
 */
int sendString(unsigned char devid, char* fmt, ...); /* return # of bytes sent */
int recvString(char* fmt, ...); /* return # of bytes received */

int sendi(int mode, unsigned char devid, char* fmt, ...); /* return # of bytes sent */
int recvi(int mode, unsigned char devid, char* fmt, ...); /* return # of bytes received */

int command(int dest, char command, int argument);

/* variables go here */
char buffer[STRING_SIZE];


#endif /* __UWMODEM_API__ */
