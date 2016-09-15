/*	-----------------------------------
*	main.c
*	SetsunaIPS
*	IPS File Patching and Creation Tool
*	Licensed with the MIT License
*
*	Copyright (c) 2016 Patrick Cland
*	www.setsunasoft.com
*	----------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <IUP/iup.h>
#include "IPS.h"
#include "file.h"
#include "callback.h"

int main(int argc, char* argv[])
{
	Ihandle* mainDialog;
	Ihandle* labelTitle, *separator;
	Ihandle* labelSrc, *labelDst, *labelSec;
	Ihandle* buttonSrc, *buttonDst, *buttonSec;
	Ihandle* hbTextButtonSrc, *hbTextButtonDst, *hbTextButtonSec;
	Ihandle* srcZone, *dstZone, *secZone;

	Ihandle* buttonPatch, *buttonCreate, *buttonAbout;
	Ihandle* actionButtonZone;

	Ihandle* tickVerbose;

	Ihandle* fileControlZone;
	Ihandle* fileAndActionButtonZone;

	Ihandle* mainWorkspace;
	
	
	IupOpen(&argc,&argv);

	/* Create separator handle */
	separator = IupLabel(NULL);
		IupSetAttribute(separator,"SEPARATOR","HORIZONTAL");

	/* Set label for title and logo */
	labelTitle = IupLabel("IPS File Patching and Creation Program");
		IupSetAttribute(labelTitle,"PADDING","8x5");
  
	/* Source file area */
	labelSrc = IupLabel("IPS Patch File | Original File");
	textSrc = IupText(NULL);
		IupSetAttribute(textSrc,"VISIBLECOLUMNS","15");
	buttonSrc = IupButton(" ... ",NULL);
	hbTextButtonSrc = IupHbox(textSrc,buttonSrc,NULL);
		IupSetAttribute(hbTextButtonSrc,"GAP","10");
	srcZone = IupVbox(labelSrc,hbTextButtonSrc,NULL);
	/* Seconday Source Area*/
	labelSec = IupLabel("Modified File");
	textSec = IupText(NULL);
		IupSetAttribute(textSec,"VISIBLECOLUMNS","15");
	buttonSec = IupButton(" ... ",NULL);
	hbTextButtonSec = IupHbox(textSec,buttonSec,NULL);
		IupSetAttribute(hbTextButtonSec,"GAP","10");
	secZone = IupVbox(labelSec,hbTextButtonSec,NULL);
	/* Destination file area */
	labelDst = IupLabel("Destination File");
	textDst = IupText(NULL);
		IupSetAttribute(textDst,"VISIBLECOLUMNS","15");
	buttonDst = IupButton(" ... ",NULL);
	hbTextButtonDst = IupHbox(textDst,buttonDst,NULL);
		IupSetAttribute(hbTextButtonDst,"GAP","10");
	dstZone = IupVbox(labelDst,hbTextButtonDst,NULL);


	/* Create verbose output tickbox */
	tickVerbose = IupToggle("Verbose Output to Log File",NULL);
		IupSetAttribute(tickVerbose,"EXPAND","YES");

	/* Compile all the file controls together */
	fileControlZone = IupVbox(srcZone,secZone,dstZone,tickVerbose,NULL);
		IupSetAttribute(fileControlZone,"NGAP","7");

	/* Setup Action Buttons */
	buttonPatch = IupButton("Patch File",NULL);
		IupSetAttribute(buttonPatch,"PADDING","19x1");
	buttonCreate = IupButton("Create Patch",NULL);
		IupSetAttribute(buttonCreate,"PADDING","9x1");
	buttonAbout = IupButton("About",NULL);
		IupSetAttribute(buttonAbout,"PADDING","30x1");
	actionButtonZone = IupVbox(buttonPatch,buttonCreate,buttonAbout,NULL);
		IupSetAttribute(actionButtonZone,"NGAP","12");
		IupSetAttribute(actionButtonZone,"MARGIN","0x20");

	fileAndActionButtonZone = IupHbox(fileControlZone,actionButtonZone,NULL);
		IupSetAttribute(fileAndActionButtonZone,"NGAP","15");
		IupSetAttribute(fileAndActionButtonZone,"MARGIN","2x2");

	/* Compile every element together to make the complete window */
	mainWorkspace = IupVbox(labelTitle,separator,fileAndActionButtonZone,NULL);
		IupSetAttribute(mainWorkspace,"NGAP","2");
		IupSetAttribute(mainWorkspace,"NMARGIN","4x3");

	mainDialog = IupDialog(mainWorkspace);
		IupSetAttribute(mainDialog,"TITLE","SetsunaIPS");
		IupSetAttribute(mainDialog,"RESIZE","NO");

	/* Setup Callbacks */
	IupSetCallback(buttonAbout,"ACTION",(Icallback)cbButtonAction);
	IupSetCallback(buttonSrc,"ACTION",(Icallback)cbButtonSrc);
	IupSetCallback(buttonDst,"ACTION",(Icallback)cbButtonDst);
	IupSetCallback(buttonSec,"ACTION",(Icallback)cbButtonSec);
	IupSetCallback(tickVerbose,"ACTION",(Icallback)cbTickVerbose);
	IupSetCallback(buttonPatch,"ACTION",(Icallback)cbButtonPatch);
	IupSetCallback(buttonCreate,"ACTION",(Icallback)cbButtonCreate);
	
	IupShowXY(mainDialog,IUP_CENTER,IUP_CENTER);

	IupMainLoop();
	IupClose();
	return EXIT_SUCCESS;
}
