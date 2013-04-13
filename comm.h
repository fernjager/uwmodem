#ifndef __UWMODEM_COMM__
#define __UWMODEM_COMM__

#include "frame.h"


void sendFrame(Frame *frame);
int recFrame(Frame *frame);

// platform dependent functions
int initComms(char* port, int baudrate);
int sendByte(unsigned char byte);
unsigned char getByte();
int cleanup();
	
#endif /* __UWMODEM_COMM__ */
