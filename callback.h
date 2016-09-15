/*	-----------------------------------
*	callback.h
*	Copyright (c) 2016 Patrick Cland
*	Licensed with the MIT License

*	www.setsunasoft.com
*	----------------------------------- */

#ifndef CALLBACK_H
#define CALLBACK_H

#include <IUP/iup.h>

/* These must be global in order to do string operations on them from callbacks */
extern Ihandle* textSrc;
extern Ihandle* textDst;
extern Ihandle* textSec;

void cbButtonAction(Ihandle* self);

void cbButtonSrc(Ihandle* self);
void cbButtonDst(Ihandle* self);
void cbButtonSec(Ihandle* self);
void cbButtonPatch(Ihandle* self);
void cbButtonCreate(Ihandle* self);

void cbTickVerbose(Ihandle* self);

void duplicateFound(void);

#endif
