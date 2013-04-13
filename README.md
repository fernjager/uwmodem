uwmodem
=======

A communications library written in C for an underwater modem.

Not thread safe, not fully optimized for embedded systems, and has a broken sliding window implementation.

Client 1
====

```c
#include <stdio.h>

#include "comm.h"
#include "uwmodem.h"

int main(){
  
	char buffers[255];
	int x = 100; int y = 200; int z = 300;

	initComms("COM4", 300);

	sendString(0x41, "x:%d, y:%d, z:%d", x, y, z);
	recvString("%s", buffers);

	printf("\n> I got the string: %s\n", buffers);

	sendString(0x41, "NO!");

	cleanup();

	return 0;
}
```

Client 2
====
```c
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
```
