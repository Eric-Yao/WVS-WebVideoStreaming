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
#ifndef	TCP_STREAMING_H_
#define	TCP_STREAMING_H_

#include "include.h"
#include "file.h"
#include "plat.h"
#include "tcp_server.h"

#define kStringLen (1024)

class Image : public TcpServer {
 public:
  Image();
  ~Image();
  void SetContentBuff(char * content_buff, unsigned int len);
 protected:
  virtual int PrepareforSend(char * send_buf);
  virtual int DealwithRecv(char * recv_buf, int len);

  bool format_media_header(char *SendBuff, int contentLen, int *SendBuffLength);
  int send_media_data(char * send_buf);

 private:
  char * content_buff_;
  int content_buff_len_;
  char image_header_[kStringLen];
  char file_name_[kStringLen];
  int counter_;
  int sent_max_size_;
  int sent_min_size_;
  long sent_total_size_;
  long sent_nos_;
};

#endif	 

/* ---END--- */


