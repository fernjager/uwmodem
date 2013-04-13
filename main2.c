#include <stdio.h>

#include "comm.h"
#include "uwmodem.h"


int main(){

	int x, y, z;
	char buffers[255];
	
	initComms("COM5", 300);


	recvString("x:%d, y:%d, z:%d", &x, &y, &z);
	sendString(0x41, "MORE");
	recvString("%s", buffers);

	printf("\n> I got the positions: x:%d, y:%d, z:%d\n", x, y, z);
	printf("\n> The remote client responded with the string: %s\n", buffers);

	cleanup();

	return 0;
}


