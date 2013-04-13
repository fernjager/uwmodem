#ifndef __UWMODEM_FRAME__
#define __UWMODEM_FRAME__

#include <inttypes.h>


/**
 * A structure representing all the fields in a frame.
 */
struct frame{
	
	/*@{*/
	unsigned char type; /**< 8 bit type field */
	unsigned char from; /**< 8 bit origin address field */
	unsigned char to; /**< 8 bit destination address field  */
	unsigned char counter; /**< 8 bit frame counter used by ARQ */
	unsigned char data[4]; /**< 32 bit data field for the frame */
	unsigned char CRC; /**< 8 bit Cyclic Redundancy Check for the frame  */
};

typedef struct frame Frame;

/**
 * s2frames - This function 
 * @param[out] x the modified input
 * @param[out] x the modified input
 * @return \f\$x + 1\f\$
 */
int s2frames(
			unsigned char type, 
			unsigned char from, 
			unsigned char to,
			char* data				
			);
			
void frames2s(int devid, char* targetBuf);

int frameString(
				int mode,
				unsigned char devid, 
				char* sourceBuf
				);
			
void CRC_Write(Frame *frame);
int CRC_Verify(Frame *frame);

void deepCopy(Frame *from, Frame *to);

#endif /* __UWMODEM_FRAME__ */
