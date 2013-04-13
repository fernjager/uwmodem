#ifndef __UWMODEM_QUEUE__
#define __UWMODEM_QUEUE__

#include "frame.h"
#include "defines.h"

extern int headTXF, tailTXF;
extern int headRXF, tailRXF;

extern Frame frameTXQueue[STACK_SIZE];
extern Frame frameRXQueue[STACK_SIZE];

int appendFrame(Frame *frame, Frame *queue, int *head, int *tail);
int firstFrame(Frame *result, Frame *queue, int *head, int *tail);
void clearFrames(int *head, int *tail);
int getSize(int head, int tail);


#endif /* __UWMODEM_QUEUE__ */
