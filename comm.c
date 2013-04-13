/* Interface*/
#include <string.h>
#include <stdio.h>
#include <windows.h>

#include "defines.h"
#include "queue.h"
#include "comm.h"
//#include <time.h>
//#include <stdlib.h>

//sendFrame
//recvFrame

DWORD dwBytesRead = 0;
DCB dcbSerialParams = {0};
HANDLE hSerial;


int initComms(char* port, int baudrate){
// implement platform dependent stuff here:
//srand((unsigned)time(0));

hSerial = CreateFile(port,
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
	
	//COMMTIMEOUTS timeouts={0};
	//timeouts.ReadIntervalTimeout=MAXDWORD;
	//timeouts.ReadTotalTimeoutConstant=0;
	//timeouts.ReadTotalTimeoutMultiplier=0;
	//timeouts.WriteTotalTimeoutConstant=50;
	//timeouts.WriteTotalTimeoutMultiplier=10;
	//if(!SetCommTimeouts(hSerial, &timeouts)){
	//	printf("Error setting state!");
	//}


}

unsigned char getByte(){
// implement platform dependent stuff here:
	char charBuff[1] = {0};

	if(!ReadFile(hSerial, charBuff, 1, &dwBytesRead, NULL)){
		printf("Error reading!");
	}
	
	return (unsigned char) charBuff[0];


}

int sendByte(unsigned char byte){
// implement platform dependent stuff here:
	char charBuff[1] = {byte};

	if(!WriteFile(hSerial, charBuff, 1, &dwBytesRead, NULL)){
		printf("Error writing!");
		return 0;
	}
	
	return 1;
}

void sendFrame(Frame *frame){
	

	CRC_Write(frame);
	
	
	#if DEBUG
	printf("comm.c: sendFrame() -> | 0x%2X 0x%2X 0x%2X 0x%2X | 0x%2X 0x%2X 0x%2X 0x%2X |  0x%2X | \n", frame->type, frame->from, frame->to, frame->counter, frame->data[0],frame->data[1], frame->data[2], frame->data[3], frame->CRC );
	#endif

	// send in big endian order
	sendByte(frame->type);
	sendByte(frame->from);
	sendByte(frame->to);
	sendByte(frame->counter);
	
	sendByte(frame->data[0]);
	sendByte(frame->data[1]);
	sendByte(frame->data[2]);
	sendByte(frame->data[3]);
	sendByte(frame->CRC);
	
	

}

int recFrame(Frame *frame){
	/* get a frame of bytes */
	/* warning: may stall, need timeout */
	
	frame->type = getByte();
	frame->from = getByte();
	frame->to = getByte();
	frame->counter = getByte();
	frame->data[0] = getByte();
	frame->data[1] = getByte();
	frame->data[2] = getByte();
	frame->data[3] = getByte();
	frame->CRC = getByte();
	
	#if DEBUG
	printf("comm.c: recFrame() -> | 0x%2X 0x%2X 0x%2X 0x%2X | 0x%2X 0x%2X 0x%2X 0x%2X |  0x%2X |", frame->type, frame->from, frame->to, frame->counter, frame->data[0],frame->data[1], frame->data[2], frame->data[3], frame->CRC );
	
	if(CRC_Verify(frame)){
		printf(" OK! \n");
	}else{
		printf(" BAD! \n");
	}
	
	#endif
	
	return CRC_Verify(frame);
	

}



int cleanup(){
	CloseHandle(hSerial);
}
// Frames are ready to be read in
// getByte
// sendByte

//sendWindow()


//shift down the array until the CRC works

/* Blocks until bitstream is synchronized */
/*
void synchronize(){
	// 18 byte buffer 
	unsigned char byteBuffer[18];
	// current offset 
	int shiftLength = 0;
	
	// get 144 bits of data, or 18 bytes 
	for(int i = 0; i < 18; i++){
		byteBuffer[i] = getByte();
	}
	
	// break into 144 bits 
	for(int i =0; i < 18; i++){
		for(int j = 0; j < 144; j++){
			for(int k = 0; k < 8; k++){
				// start with MSB, and shift downwards
				byteBuffer[i] = (( getByte() >> (8-k)) && 0x01);
			}
		}
	}
	
}
*/
