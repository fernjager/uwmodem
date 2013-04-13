// The 1-Wire CRC scheme is described in Maxim Application Note 27:
// "Understanding and Using Cyclic Redundancy Checks with Maxim iButton Products"
// Adopted from http://github.com/paeaetech/paeae/tree/master/Libraries/ds2482/

#include "crc.h"

unsigned char crc8( unsigned char *addr, unsigned char len)
{
	unsigned char crc=0;
	int i = 0, j = 10;
	
	for (i=0; i<len;i++)
	{
		unsigned char inbyte = (unsigned char)addr[i];
		for (j=0;j<8;j++)
		{
			unsigned char mix = (crc ^ inbyte) & 0x01;
			crc >>= 1;
			if (mix)
				crc ^= 0x8C;

			inbyte >>= 1;
		}
	}
	return (unsigned char)crc;
} 
