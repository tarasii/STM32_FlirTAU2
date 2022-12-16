#include "crc.h"

//#define POLY 0x8408
//#define    CRC_POLY_CCITT    0x1021
#define    POLY    0x1021

uint16_t crc16(uint8_t* data_p, uint8_t length)
{
	uint8_t i;
	//uint16_t data;
	uint16_t crc;

	crc = 0x0;
			
//	do {
//		for (i = 0, data = ((uint16_t)0xff & *data_p++)<<8; i < 8; i++, data <<= 1) {
//			if ((crc & 0x8000) ^ (data & 0x8000))
//				crc = (crc << 1) ^ POLY;
//			else
//				crc <<= 1;					
//		}
//	} while (--length);
			 
	while (length--) {
		crc ^= *(uint16_t *)data_p++ << 8;
		for (i=0; i < 8; i++)
			//crc = crc & 0x8000 ? (crc << 1) ^ 0x1021 : crc << 1;
			if (crc & 0x8000)
				crc = (crc << 1) ^ POLY;
			else
				crc <<= 1;
	}
		
	return crc & 0xffff;
}

