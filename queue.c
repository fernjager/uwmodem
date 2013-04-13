#include "defines.h"
#include "queue.h"
#include "frame.h"
#include "uwmodem.h"

#if DEBUG
	#include <stdio.h>
#endif
/* Woohoo! Constant time Queue array implementation */


int appendFrame( Frame *frame, Frame* queue, int* head, int* tail){

	
	#if DEBUG
		printf("queue.c: appendFrame() -> 0x%2X 0x%2X 0x%2X 0x%2X \n", frame->data[0], frame->data[1], frame->data[2], frame->data[3]);
		
		printf("queue.c: appendFrame() -> %c    %c     %c    %c \n", frame->data[0], frame->data[1], frame->data[2], frame->data[3]);
		
	#endif
	
	if( (*tail) >= (*head) + STACK_SIZE )
		return 0;
		
	deepCopy(frame, &(queue[*tail]));
	
	/* restrict within STACK_SIZE */
	(*tail) = (*tail + 1) % STACK_SIZE;

	return 1;
}

int firstFrame(Frame * result, Frame *queue, int* head, int* tail){

	
	if (*tail == *head){
		return 0;
	}
		
	deepCopy(&(queue[*head]), result);
	
	(*head)++;
	(*head) %= STACK_SIZE;
	

}

int getSize(int head, int tail){
	return (tail+STACK_SIZE-head) % STACK_SIZE;
}
