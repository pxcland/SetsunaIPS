/*	-----------------------------------
*	IPS.h
*	Copyright (c) 2016 Patrick Cland
*	Licensed with the MIT License

*	www.setsunasoft.com
*	----------------------------------- */

#ifndef IPS_H
#define IPS_H

#define _EOF 0x454F46

#include <stdio.h>
#include <stdint.h>

void swapEndian16(uint16_t* x);
void swapEndian32(uint32_t* x);
void _32to24(uint32_t* x);
int assembleRecord(FILE** dst, FILE** src, uint32_t rOffset, uint16_t rSize);

#endif
