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













/*
int i=0, n = 0;
char charBuff[1] = {0};
char szBuff[10] = {0};

DWORD dwBytesRead = 0;
DCB dcbSerialParams = {0};
HANDLE hSerial;

hSerial = CreateFile("COM7",
					GENERIC_READ | GENERIC_WRITE,
					0,
					0,
					OPEN_EXISTING,
					FILE_ATTRIBUTE_NORMAL,
					0);
	if(hSerial==INVALID_HANDLE_VALUE){
		if(GetLastError()==ERROR_FILE_NOT_FOUND){
		//serial port does not exist. Inform user.
		printf("Serial port does not exist!");
		}
		//some other error occurred. Inform user.
		printf("Error opening serial port!");
	}



	if (!GetCommState(hSerial, &dcbSerialParams)) {
	//error getting state
	printf("Error getting state!");
	}
	dcbSerialParams.BaudRate=CBR_300;
	dcbSerialParams.ByteSize=8;
	dcbSerialParams.StopBits=ONESTOPBIT;
	dcbSerialParams.Parity=NOPARITY;
	
	if(!SetCommState(hSerial, &dcbSerialParams)){
	printf("Error setting state!");
	}
	
	
	
while(n < 9){
if(!ReadFile(hSerial, charBuff, 1, &dwBytesRead, NULL)){
		printf("Error reading!");
}

szBuff[n] = charBuff[0];
n++;
}

szBuff[n] = '\0';


printf("9 bytes: %s\n\n", szBuff);
printf("9 bytes in order of 0 - 9: ");
printf("0x%2X ", szBuff[0]); // should be the packet type
printf("0x%2X ", szBuff[1]); // followed by from
printf("0x%2X ", szBuff[2]); // followed by to
printf("0x%2X ", szBuff[3]); // followed by counter
printf(" | "); 

for(i = 0; i < 4; i++)
	printf("0x%2X ", szBuff[4+i]); // followed by data 4 bytes
	
printf(" | "); 
printf("0x%2X ", szBuff[8]); // followed by CRC


CloseHandle(hSerial);
*/



/*
while(n < 9){
	
	szBuff[n] = getByte();
	n++;
}

printf("9 bytes in order of 0 - 9: ");
printf("0x%2X ", szBuff[0]); // should be the packet type
printf("0x%2X ", szBuff[1]); // followed by from
printf("0x%2X ", szBuff[2]); // followed by to
printf("0x%2X ", szBuff[3]); // followed by counter
printf(" | "); 

for(i = 0; i < 4; i++)
	printf("0x%2X ", szBuff[4+i]); // followed by data 4 bytes
	
printf(" | "); 
printf("0x%2X ", szBuff[8]); // followed by CRC
*/