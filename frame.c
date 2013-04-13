#include <string.h>


#include "defines.h"
#include "frame.h"
#include "queue.h"
#include "protocol.h"
#include "crc.h"

#if DEBUG
#include <stdio.h>
#endif


int headTXF, tailTXF;
int headRXF, tailRXF;
Frame frameTXQueue[STACK_SIZE];
Frame frameRXQueue[STACK_SIZE];

void deepCopy(Frame *from, Frame *to){

		to->type = from->type;
		to->to = from->to;
		to->counter = from->counter;
		
		to->data[0] = from->data[0];
		to->data[1] = from->data[1];
		to->data[2] = from->data[2];
		to->data[3] = from->data[3];
		
		to->CRC = from->CRC;
		
}

int s2frames(
			unsigned char type, 
			unsigned char from, 
			unsigned char to,
			char* data
			){
	
	
	int i =0, j= 0;
	Frame temp;
	
	#ifdef DEBUG
		printf("frame.c: s2frames() -> we need to break it up into frames\n");
	#endif
	
	for(i = 0; i<strlen(data); i+=4){
		temp.type = type;
		temp.from = from;
		temp.to = to;
		
		for(j = 0; j < 4; j++){
			
			// If we hit null character, pad the rest with null characters
			// construct frames until we hit null character
			// This works, since it performs a soft copy
			// of the local struct onto the one in the global
			// array.
			
			if(data[i+j] == '\0'){
				
				for(; j < 4; j ++)
					temp.data[j] = '\0';
				
				// break out of both inner and outer loops
				i = strlen(data);
				break;
				
			}else{
				temp.data[j] = data[i+j];
			}
			
			
		}
		
			
			appendFrame(&temp, frameTXQueue, &headTXF, &tailTXF);
	}
	
	
	/* flush, send all , this is only in blocking mode,
	 * if nonblocking mode, we simply leave stuff on queue */
	frameTxARQ();

}


void frames2s(int devid, char* targetBuf){
	// Get frame data until we hit null character
	
	Frame currFrame;
	unsigned char frameDataByte = 0xFF;
	int bytes = 0;
	int bufCounter = 0;
	int done = 0;
	
	#ifdef DEBUG
		printf("frame.c: frames2s() -> waiting for a window of frames\n");
	#endif
	
	
	while (!done){
	
		/* get window of frames */
		frameRxARQ();
		
		#ifdef DEBUG
		printf("frame.c: frames2s() -> Processing frames to string\n");
		#endif
	
		
		/* process received frames */
		// retrieve a frame, simple copy is allowed here
		/* Get all DATA or DATAI frames from stack and turn into string */
		while(getSize(headRXF, tailRXF) > 0){
			firstFrame(&currFrame, frameRXQueue, &headRXF, &tailRXF);
		
		
			// If it's not data we need, put it back on the queue and continue
			if(currFrame.type != DATA){
				appendFrame(&currFrame, frameRXQueue, &headRXF, &tailRXF);
				continue;
			}
		
			// if this packet is not addressed to us, just ignore it
			// skip adding its data contents to the queue
			if(currFrame.to != devid)
				continue;
		
			// extract all 4 bytes of payload data
			for (bytes =0; bytes < 4; bytes++){
	
				frameDataByte = currFrame.data[bytes];
			
				/* build up our buffer... */
				targetBuf[bufCounter++] = frameDataByte;
			
				/* We've hit the end of the string, and are done */
				if(frameDataByte == '\0') done = 1;
		
			}
		}
	}
	
}

int frameString(
				int mode,
				unsigned char devid, 
				char* sourceBuf
				){
	
	if(sourceBuf == NULL) return 0;
	
	#ifdef DEBUG
		printf("frame.c: frameString() -> deciding what priority the frame will be.\n");
	#endif
	
	switch(mode){
	
		case NORMAL_PRIORITY:
			return s2frames( DATA, SELF_ID, devid, sourceBuf );
		break;
		
		case HIGH_PRIORITY:
			return s2frames( DATAI, SELF_ID, devid, sourceBuf );
		break;
		
		case NO_ACK:
			return s2frames( DATANOACK, SELF_ID, devid, sourceBuf );
		break;
		
	}

}

void CRC_Write(Frame *frame)
{
/* warning: dependent on data field size */
unsigned char CRCBuf[9];

	CRCBuf[0] = frame->type;
	CRCBuf[1] = frame->from;
	CRCBuf[2] = frame->to;
	CRCBuf[3] = frame->counter;
	CRCBuf[4] = frame->data[0];
	CRCBuf[5] = frame->data[1];
	CRCBuf[6] = frame->data[2];
	CRCBuf[7] = frame->data[3];

	/* run array through CRC and assign resulting CRC to
	* to the CRC field */
 
	frame->CRC = crc8(CRCBuf, 0x08);
	
	#if DEBUG
	// mess up CRC to simulate corrupted packets
	// 1 in 10
	//if((rand() % 10 + 0) == 5)
	//	frame->CRC = 0x00;
	#endif
	
}

int CRC_Verify(Frame *frame)
{
/* warning: dependent on data field size */
unsigned char CRCBuf[9];

/* build up array */
CRCBuf[0] = frame->type;
CRCBuf[1] = frame->from;
CRCBuf[2] = frame->to;
CRCBuf[3] = frame->counter;
CRCBuf[4] = frame->data[0];
CRCBuf[5] = frame->data[1];
CRCBuf[6] = frame->data[2];
CRCBuf[7] = frame->data[3];

	
/* run array through CRC */
/* equal? */

return (crc8(CRCBuf, 0x08) == frame->CRC);

}