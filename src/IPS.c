/*	-----------------------------------
*	IPS.c
*	Copyright (c) 2016 Patrick Cland
*	Licensed with the MIT License

*	www.setsunasoft.com
*	----------------------------------- */

#include <malloc.h>
#include "IPS.h"
#include "file.h"

void swapEndian16(uint16_t* x)
{
	*x =((*x >> 8) | (*x << 8));
}

void swapEndian32(uint32_t* x)
{
	*x = ((*x >> 24) | ((*x << 8) & 0x00FF0000) | ((*x >> 8) & 0x0000FF00) | (*x << 24));
}
void _32to24(uint32_t* x)
{
	*x = ((*x >> 8) & 0x00FFFFFF);
}

int assembleRecord(FILE** dst, FILE** src, uint32_t rOffset, uint16_t rSize)
{
	uint32_t to = rOffset;
	uint16_t ts = rSize;
	uint8_t* data;
	long srcPos = ftell(*src);

	swapEndian32(&to);
	_32to24(&to);
	swapEndian16(&ts);

	fseek(*src,rOffset,SEEK_SET);
	fwrite(&to,3,1,*dst);
	fwrite(&ts,2,1,*dst);
	if((data = readFileData(src,rOffset,rSize)) == NULL)
	{
		return -1;
	}
	fwrite(data,rSize,1,*dst);
	fseek(*src,srcPos,SEEK_SET);
	free(data);
	
	return 0;
}