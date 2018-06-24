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
#ifndef	TCP_SERVER_H_
#define	TCP_SERVER_H_

#include "include.h"
#include "file.h"
#include "plat.h"
#include "dd_listen.h"


class TcpServer {
 protected:
  enum CntStatus {
    kInit = 0,
    kConnected,
    kLost
  };
 public:
  TcpServer();
  ~TcpServer();
  bool Setup(int listenPort);
  void Run(char * recv_buf, char * send_buf, unsigned int & len);
 protected:
  bool CheckCnt(bool readable, bool writeable, char * recv_buf, char * send_buf, unsigned int & len);

  virtual bool IsOneTimeCnt() = 0;
  virtual int PrepareforSend(char * send_buf) = 0;
  virtual int DealwithRecv(char * recv_buf, int len) = 0;

  int client_socket() {return client_socket_;}

 private:
  CntStatus cnt_status_;
  timeval  timeout_;
  Listen*  listen_;
  fd_set   read_set_;
  fd_set   write_set_;
  fd_set   err_set_;
  int      client_socket_;
  int      check_cnt_nos_;
};

#endif	 

/* ---END--- */


