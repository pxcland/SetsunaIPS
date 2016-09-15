/*	-----------------------------------
*	file.c
*	Copyright (c) 2016 Patrick Cland
*	Licensed with the MIT License

*	www.setsunasoft.com
*	----------------------------------- */

#include <malloc.h>
#include <IUP/iup.h>
#include "file.h"
#include "IPS.h"

/* Global variable for whether the user wants verbose output to a log file */
/* Global in order to be accessable from the toggle UI element callback */
int verbose = -1;

uint8_t* readFileData(FILE** f, long offset, int length)
{
	uint8_t* tmp = malloc(length);
	if(tmp == NULL)
	{
		return NULL;
	}
	fseek(*f,offset,SEEK_SET);
	fread(tmp,length,1,*f);
	return tmp;
}
