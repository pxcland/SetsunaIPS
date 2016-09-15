/*	-----------------------------------
*	file.h
*	Copyright (c) 2016 Patrick Cland
*	Licensed with the MIT License

*	www.setsunasoft.com
*	----------------------------------- */

#ifndef FILE_H
#define FILE_H

#include <stdint.h>
#include "IPS.h"

extern int verbose;

uint8_t* readFileData(FILE** f, long offset, int length);

#endif
