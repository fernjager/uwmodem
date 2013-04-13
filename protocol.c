
#include "defines.h"
#include "frame.h"
#include "queue.h"
#include "protocol.h"
#include "comm.h"

#if DEBUG
#include <stdio.h>
#endif 


Frame windowTX[WINDOW_SIZE];
Frame windowRX[WINDOW_SIZE];


/* build and sends ALL frames on the frameTxQueue
 * Uses global window defined in protocol.h */
 
void frameTxARQ(){
	int i = 0;
	Frame padding;
	Frame tempFrame;
	char pad[9];
	
	/* frame count of the current sending window */
	int frameCount = 0;
	
	/* count of return ACKS we got */
	int recCount= 0;
	
	/* timeout before we send our data again */
	int timeoutCount = 0;
	
	int done = 0;
	
	/* make padding frame with no data */
	padding.type = 0x01;
	
	
	for(i=0; i < 4; i++)
		padding.data[i] =0;
	
	
		#if DEBUG
		printf("protocol.c: frameTxARQ() -> we got %d frames on the stack to send\n", getSize(headTXF, tailTXF));
		#endif
		
	while(getSize(headTXF, tailTXF) != 0){
	
		for( i = 0; i < WINDOW_SIZE; i++ )
		{
			/* fill the rest of the window with 
			 * padding frames to pad up to window size
			 * if we have nothing left to send */
			if( getSize(headTXF, tailTXF) == 0 ){
				deepCopy(&padding, &windowTX[i]);
				windowTX[i].counter = frameCount++;
				
			}else{
				firstFrame(&windowTX[i], frameTXQueue, &headTXF, &tailTXF);
				windowTX[i].counter = frameCount++;
				/* get the destination for our padding */
				/* bug point? might fail with empty string */
				padding.to = windowTX[i].to;
				padding.from = windowTX[i].from;
			}
			
		}

		#if DEBUG
		printf("protocol.c: frameTxARQ() -> we built our window\n", getSize(headTXF, tailTXF));
		#endif
		
		/* Send the window */
		for(i = 0; i < WINDOW_SIZE; i++)
		{
			sendFrame(&windowTX[i]);
		}
		
		#if DEBUG
		printf("protocol.c: frameTxARQ() -> %d frames in window sent\n", i);
		#endif
		
		/* clear our receive window in preparation */
		for(i = 0; i < WINDOW_SIZE; i++)
		{
			windowRX[i].type = 0x00;
		}
		
		#if DEBUG
		printf("protocol.c: frameTxARQ() -> awaiting window ACKS\n");
		#endif
		
		/* Now get our ACKS */
		while(!done)
		{
			/* receive a frame of data */
			/* returns one if CRC is good */
			if (recFrame(&tempFrame)){
			
				/* if it is an ACK frame */
				if(tempFrame.type == ACK){
				
					/* if ACK frame already is not in our 
					 * receive window already, we increase recCount */
					 
					if(windowRX[tempFrame.counter].type == 0x00)
						recCount++;
					
					deepCopy(&tempFrame, &windowRX[tempFrame.counter]);
					
					
					/* we got all our ACKS */
					if(recCount == WINDOW_SIZE){
						/* reset recCount */
						recCount = 0;
						
						/* new window, reset frame count */
						frameCount = 0;
						
						/* reset timeout */
						timeoutCount = 0;
						
						break;
					}
					/* we didn't get all our ACKS */
					else{
					
						timeoutCount++;
						
					}
					
				}
				
			} /* we didn't get an ACK, drop packet and increment timeout */
			else{
				timeoutCount++;
			}
			
			/* here we check if we timed out. If we did, 
			 * resend the frames that we did not get an ACK for */
			if(timeoutCount == TIMEOUTCYCLES){
				for(i = 0; i < WINDOW_SIZE; i ++){
					if(windowRX[i].type == 0x00)
						sendFrame(&windowTX[i]);
				}
			 }
			 
		}
		
		#if DEBUG
		printf("\nprotocol.c: frameTxARQ() -> remote host has received our packets successfully!\n");
		#endif
	}
	
}


/* get ONE window of frames and add it to the frameRxQueue*/
void frameRxARQ(){
	
	int i = 0;
	Frame tempFrame;
	Frame ACKFrame;
	
	/* count of received frames */
	int frameCount = 0;
	
	/* blank out our ACKFrame */
	for(i = 0; i < 4; i ++)
	{
		ACKFrame.data[i] = 0x00;
	}
	
	/* clear our receive window in preparation */
	for(i = 0; i < WINDOW_SIZE; i++)
	{
		windowRX[i].type = 0x00;
	}
	
	/* Now get our window */
	while( frameCount < WINDOW_SIZE)
	{
		#ifdef DEBUG
		printf("protocol.c: frameRxARQ() -> waiting for frames %d/10...\n", frameCount+1);
		#endif
	
		/* receive a frame of data */
		/* returns one if CRC is good */
		if (recFrame(&tempFrame)){
		
			/* if it is a DATA frame Add security check on counter */
			if(tempFrame.type == DATA && tempFrame.counter < WINDOW_SIZE ){
			
				/* if we haven't received it before*/
				if(windowRX[tempFrame.counter].type == 0x00)
					frameCount++;
				
				/* Here be dragons */
				deepCopy(&tempFrame, &windowRX[tempFrame.counter]);
				
				/* Send ACK regardless of whether or 
				 * not we've received it before*/
				
				/* build our ACK Frame */
				ACKFrame.type = ACK;
				
				/* the count is that of what we received */
				ACKFrame.counter = tempFrame.counter;
				
				/* send the ACK */
				sendFrame(&ACKFrame);
				
			}
			
		} 
		/* NOTE: Handle DATAI and NOACK frames */
		else{
			
			#ifdef DEBUG
			printf("protocol.c: frameRxARQ() -> CRC Failed...\n");
			#endif
		}
		 
	}
	/* reset frameCount */
	frameCount = 0;
	#ifdef DEBUG
	printf("protocol.c: frameRxARQ() -> Receive complete...\n");
	#endif
	
	/* put all the frames in the window
	 * to the rxQueue*/
	for(i = 0; i < WINDOW_SIZE; i++)
		appendFrame( &windowRX[i], frameRXQueue, &headRXF, &tailRXF);
	
}
