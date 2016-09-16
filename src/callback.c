/*	-----------------------------------
*	callback.c
*	Copyright (c) 2016 Patrick Cland
*	Licensed with the MIT License

*	www.setsunasoft.com
*	----------------------------------- */

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <malloc.h>
#include <stdint.h>
#include <string.h>
#include "callback.h"
#include "file.h"

Ihandle* textSrc;
Ihandle* textDst;
Ihandle* textSec;

void cbButtonAction(Ihandle* self)
{
	Ihandle* dlg, *label, *sep, *label2, *vbox;
	
	label	= IupLabel("SetsunaIPS 2.0.1");
	sep		= IupLabel("");
		IupSetAttribute(sep,"SEPARATOR","HORIZONTAL");
	label2	= IupLabel("IPS File Patcher and Creation Tool - Copyright 2016 Patrick Cland\nwww.setsunasoft.com");
	vbox	= IupVbox(label,sep,label2,NULL);
		IupSetAttribute(vbox,"NGAP","5");
	dlg		= IupDialog(vbox);
		IupSetAttribute(dlg,"TITLE","About");
		IupSetAttribute(dlg,"RESIZE","NO");
		IupSetAttribute(dlg,"MARGIN","8x8");

	IupPopup(dlg,IUP_CURRENT,IUP_CURRENT);
	IupDestroy(dlg); IupDestroy(label); IupDestroy(sep); IupDestroy(label2); IupDestroy(vbox);
}

void cbButtonSrc(Ihandle* self)
{
	Ihandle* filedlg = IupFileDlg();
		IupSetAttribute(filedlg,"DIALOGTYPE","OPEN");
		IupSetAttribute(filedlg,"EXTFILTER","IPS Patches|*.ips|All Files|*.*|");

	IupPopup(filedlg,IUP_CURRENT,IUP_CURRENT);
	
	/* Populate text box for path with the filename selected */
	IupSetAttribute(textSrc,"VALUE",IupGetAttribute(filedlg,"VALUE"));
	IupDestroy(filedlg);
}

void cbButtonDst(Ihandle* self)
{
	Ihandle* filedlg = IupFileDlg();
		IupSetAttribute(filedlg,"DIALOGTYPE","OPEN");
		IupSetAttribute(filedlg,"EXTFILTER","All Files|*.*|");
		IupSetAttribute(filedlg,"ALLOWNEW","YES");
		
	IupPopup(filedlg,IUP_CURRENT,IUP_CURRENT);
	
	/* Populate text box for path with the filename selected */
	IupSetAttribute(textDst,"VALUE",IupGetAttribute(filedlg,"VALUE"));
	IupDestroy(filedlg);
}

void cbButtonSec(Ihandle* self)
{
	Ihandle* filedlg = IupFileDlg();
		IupSetAttribute(filedlg,"DIALOGTYPE","OPEN");
		IupSetAttribute(filedlg,"EXTFILTER","All Files|*.*|");
		IupSetAttribute(filedlg,"ALLOWNEW","YES");
		
	IupPopup(filedlg,IUP_CURRENT,IUP_CURRENT);
	
	/* Populate text box for path with the filename selected */
	IupSetAttribute(textSec,"VALUE",IupGetAttribute(filedlg,"VALUE"));
	IupDestroy(filedlg);
}

void cbTickVerbose(Ihandle* self)
{
	verbose *= -1;
}

void cbButtonPatch(Ihandle* self)
{
	FILE* src, *dst, *log;
	char signature[5] = { 0 };
	uint32_t	offset;
	uint16_t	size;
	uint8_t		data;
	uint8_t		dataRLE;
	int i, j, isEncoded = 0;

	if(!strcmp(IupGetAttribute(textSrc,"VALUE"),"") || !strcmp(IupGetAttribute(textDst,"VALUE"),""))
	{
		IupMessage("Error","File(s) not specified!");
		return;
	}
	if(!strcmp(IupGetAttribute(textSrc,"VALUE"),IupGetAttribute(textDst,"VALUE")))
	{
		duplicateFound();
		return;
	}
	if((src = fopen(IupGetAttribute(textSrc,"VALUE"),"rb")) == NULL)
	{
		IupMessage("Error","Can't open specified source file!");
		return;
	}
	if((dst = fopen(IupGetAttribute(textDst,"VALUE"),"rb+")) == NULL)
	{
		IupMessage("Error","Can't open specified destination file!");
		fclose(src);
		return;
	}

	if(verbose > 0)
	{
		if((log = fopen("log.txt","w")) == NULL)
		{
			IupMessage("Error","Can't open log file!");
			fclose(src);
			fclose(dst);
			return;
		}
		fprintf(log,"Opened log file.\nOperating with source file %s.\nOperating with destination file %s.\nApplying patch.\n",
			IupGetAttribute(textSrc,"VALUE"),
			IupGetAttribute(textDst,"VALUE"));
	}

	/* Begin file patching */
	fseek(src,0,SEEK_SET);
	fseek(src,0,SEEK_SET);

	/* Signature must be PATCH to be considered a valid file */
	fread(&signature[0],5,1,src);
	if(strncmp(signature,"PATCH",5) != 0)
	{
		IupMessage("Error","Source file is not a valid IPS patch!");
		if(verbose > 0)
		{
			fprintf(log,"Source file is not a valid IPS patch.");
			fclose(log);
		}
		fclose(src);
		fclose(dst);
	}
	
	/* Values are stored big endian in the file */
	/* Read the offset from the record in the file */
	fread(&offset,3,1,src);
	swapEndian32(&offset);
	_32to24(&offset);
	while(offset != _EOF)
	{
		/* Move to the offset in the destination file to begin writing */
		fseek(dst,offset,SEEK_SET);

		fread(&size,2,1,src);
		swapEndian16(&size);

		/* A size of 0 indicates the record is RLE encoded */
		if(!size)
		{
			if(verbose > 0)
				fprintf(log,"\nFound an encoded run.");
			isEncoded = 1;
			/* The size of the RLE run is stored in the next 2 bytes */
			fread(&size,2,1,src);
			swapEndian16(&size);
		}

		if(verbose > 0)
			fprintf(log,"\n\nWriting %d bytes to offset 0x%08X\n",size,offset);

		for(i = 0; i < size; i++)
		{
			if(isEncoded)
			{
				fread(&dataRLE,1,1,src);
				for(j = 0; j < size; j++)
				{
					fwrite(&dataRLE,1,1,dst);
					if(verbose > 0)
						fprintf(log,"%X",data);
				}
				isEncoded = 0;
				break;
			}

			fread(&data,1,1,src);
			fwrite(&data,1,1,dst);
			if(verbose > 0)
				fprintf(log,"%X",data);
		}
		fread(&offset,3,1,src);
		swapEndian32(&offset);
		_32to24(&offset);
	}

	IupMessage("","Successfully applied patch!");

	fclose(src);
	fclose(dst);
	if(verbose > 0)
	{
		fclose(log);
	}
}

void cbButtonCreate(Ihandle* self)
{
	FILE* src, *src2, *dst, *log;
	char* signature = "PATCH";
	char* end		= "EOF";
	uint32_t	offset = 0;
	uint16_t	size = 0;
	uint8_t		byteSrc, byteSrc2;
	int i;
	int srcLength;
	int running = 0;
	int diff = 0;

	if(!strcmp(IupGetAttribute(textSrc,"VALUE"),"") 
			|| !strcmp(IupGetAttribute(textDst,"VALUE"),"")
			|| !strcmp(IupGetAttribute(textSec,"VALUE"),""))
	{
		IupMessage("Error","File(s) not specified!");
		return;
	}
	if(	!strcmp(IupGetAttribute(textSrc,"VALUE"),IupGetAttribute(textSec,"VALUE")) ||
		!strcmp(IupGetAttribute(textSrc,"VALUE"),IupGetAttribute(textDst,"VALUE")) ||
		!strcmp(IupGetAttribute(textSec,"VALUE"),IupGetAttribute(textDst,"VALUE")))
	{
		duplicateFound();
		return;
	}
	if((src = fopen(IupGetAttribute(textSrc,"VALUE"),"rb")) == NULL)
	{
		IupMessage("Error","Can't open specified source file!");
		return;
	}
	if((src2 = fopen(IupGetAttribute(textSec,"VALUE"),"rb")) == NULL)
	{
		IupMessage("Error","Can't open specified secondary source file!");
		fclose(src);
		return;
	}
	if((dst = fopen(IupGetAttribute(textDst,"VALUE"),"wb+")) == NULL)
	{
		IupMessage("Error","Can't open specified destination file!");
		fclose(src);
		fclose(src2);
		return;
	}

	if(verbose > 0)
	{
		if((log = fopen("log.txt","w")) == NULL)
		{
			IupMessage("Error","Can't open log file!");
			fclose(src);
			fclose(src2);
			fclose(dst);
			return;
		}
		fprintf(log,"Opened log file.\nOperating with source file %s.\nOperating with secondary source file %s.\nOperating with destination file %s.\nCreating patch.\n",
			IupGetAttribute(textSrc,"VALUE"),
			IupGetAttribute(textSec,"VALUE"),
			IupGetAttribute(textDst,"VALUE"));
	}

	
	/* Get file length */
	fseek(src2,0,SEEK_END);
	srcLength = ftell(src2);
	/* Make sure all files are at the beginning */
	fseek(src,0,SEEK_SET);
	fseek(src2,0,SEEK_SET);
	fseek(dst,0,SEEK_SET);
	/* Write the signature to the file, to prove it is a valid file */
	fwrite(signature,5,1,dst);

	for(i = 0; i < srcLength; i++)
	{
		fread(&byteSrc,1,1,src);
		fread(&byteSrc2,1,1,src2);
		if(byteSrc2 != byteSrc)
		{
			if(!running)
			{
				offset = i;
			}
			size++;
			running = 1;
		}
		else if(byteSrc2 == byteSrc)
		{
			if(running)
			{
				if(assembleRecord(&dst,&src2,offset,size) == -1)
				{
					if(verbose > 0)
					{
						fprintf(log,"Error assembling record. Offset: 0x%X\n",offset);
						fclose(log);
					}
					IupMessage(NULL,"Error creating patch!");
					fclose(src); fclose(src2); fclose(dst);
					return;
				}

				if(verbose > 0)
					fprintf(log,"Offset:\t0x%X\t%d byte(s) different\n",offset,size);
				size = 0;
				running = 0;
			}
			else
			{
				continue;
			}
		}
	}
	
	/* All IPS patches must end with the string 'EOF' */
	fwrite(end,3,1,dst);

	IupMessage("","Successfully created patch!");
	if(verbose > 0)
	{
		fprintf(log,"\nSuccessfully created patch!");
		fclose(log);
	}
	fclose(src);
	fclose(src2);
	fclose(dst);
}

void duplicateFound(void)
{
	Ihandle* mdlg = IupMessageDlg();
		IupSetAttribute(mdlg,"DIALOGTYPE","INFORMATION");
		IupSetAttribute(mdlg,"TITLE","Error");
		IupSetAttribute(mdlg,"BUTTONS","OK");
		IupSetAttribute(mdlg,"VALUE","Source and destination files cannot be the same.");
	IupPopup(mdlg,IUP_CURRENT,IUP_CURRENT);
	IupDestroy(mdlg);
}
