/*****************************************************************************
 * FILE NAME:				file.h
 * VERSION NO.:				1.0
 * LAST TIME:				2010-10-06
 * AUTHOR:				Yong Yao
 * 
 *--------------------------------------
 * Copyright (C) 2009-2019  Yong Yao
 *--------------------------------------
 *
 * MODIFICATION HISTORY
 *	
 * Ver       Date                By                 Details
 * ---  ----------      -------           -------
 * 0.1      2010-10-06    Yong Yao		all
 *****************************************************************************/
#ifndef	_FILE_
#define	_FILE_

#include "include.h"

#define DFL_READ  1
#define DFL_WRITE  2

int get_file_size(const char *path );

bool file_read(const char *FileName, int Address, int ReadBufLen, char *ReadBuf);

bool file_write(const char *FileName,int Address, int WriteBufLen, char *WriteBuf);

bool file_create(const char *FileName);

int file_open(const char *FileName,int Type);

bool file_close(int fp);

bool file_delet(const char *path);



#endif	 

/* ---END--- */


