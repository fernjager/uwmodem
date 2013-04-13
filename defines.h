#ifndef __UWMODEM_DEF__
#define __UWMODEM_DEF__

#define WINDOWS 1
#define DEBUG 1
#define TIMEOUTCYCLES 1000
/* Stage 2: Frame Block Formation */
/* Frame Type */
/* Read Inter-frame Protocol.pdf for more details */
#define DATA 0x01
#define DATAI 0x02
#define DATANOACK 0x03
#define RAW	0x04
#define RESPONSE 0x05
#define ACK 0x06

/* USER */
#define SELF_ID 0x41


/* Raw Commands */
/* Get */
#define RESUME 0x01
#define AVGRESP 0x02

/* Set */
#define DATALENGTH 0xA1
#define CLEARSTACK 0xA2

/* MODES */
#define AUTO_MODE 0x00
#define PSK 0x01
#define ASK 0x02

/* stack settings */
#define STACK_SIZE 65
#define WINDOW_SIZE 10

#define STRING_SIZE 255

/* Platform specific settings */
#define BIG_ENDIAN 0
#define BYTE_ORDER BIG_ENDIAN

/* Send modes */
#define NO_ACK 1
#define HIGH_PRIORITY 2
#define NORMAL_PRIORITY 3


//Hardware
/* CRC error threshold before going back to synchronize */
/* How many failed acks before attempting to synchronize signal again */
/* By default, idle frames are send */
//#define SYNC_ERROR_THRESH 20


/* Intrinsic to protocol, but placed here for 
 * reference and ease of future modifications */
/* Maximum number of physical nodes in network */
#define NODES 255


#endif /* __UWMODEM_DEF__ */
