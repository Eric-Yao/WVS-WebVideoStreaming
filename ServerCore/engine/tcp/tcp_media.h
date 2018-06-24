/*****************************************************************************
 * FILE NAME:				tcp.h
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
#ifndef	MEDIA_H_
#define	MEDIA_H_

#include "include.h"
#include "plat.h"
#include "file.h"
#include "draw_apis.h" //gl_window
#include "record.h"


#define kBufferSize (1024*80)
#define kStringLen (1024)


class Media
{
public:
    Media(char * contet_buff, Record* record, pthread_mutex_t mutex);
    ~Media();
    
    
    bool setup_noblock_server(int serverport);
    bool setup_block_server(int serverport);
    
protected:
    int close_socket(int s);	
    int receive_data(int Socket);
    int send_jpg_data(int Socket);
    int send_media_data(int Socket);
    bool format_image_header(char *SendBuff, int contentLen, int *SendBuffLength);
    bool format_media_header(char *SendBuff, int contentLen, int *SendBuffLength);
    
protected:
    int m_Counter;
    int m_NoblockServer;
    int m_NoblockServerPort;

    int m_BlockServer;
    int m_BlockServerPort;


				
    fd_set   m_ReadSet;
    fd_set   m_WriteSet;
    fd_set   m_ErrSet;
    timeval    m_Timeout;


    char m_ImageHeader[kStringLen];
	char m_FileName[kStringLen];
    char m_TempBuffer[kBufferSize];

	char m_SendBuffer[kBufferSize];
	char m_ReceiveBuffer[kBufferSize];

    char* m_ContentBuff;
    Record* m_Record;
    pthread_mutex_t m_BuffMutex;

};

#endif	 

/* ---END--- */


