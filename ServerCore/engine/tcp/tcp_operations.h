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
#ifndef	TCP_OPERATIONS_H_
#define	TCP_OPERATIONS_H_

#include "include.h"
#include "file.h"
#include "plat.h"
#include "tcp_server.h"
#include "animat.h"
#include "record.h"

class Operations : public TcpServer {
 public:
  Operations(const char* log_path);
  ~Operations();
  void SetStatistics(char *buf_a, char *buf_b, int len_a, int len_b );
  void RegisterEnvironment(Animat* userobj, Record* recordobj);
  
  int move_tag();
  int rotate_tag();
  int change_quality_tag();

 protected:
  virtual int PrepareforSend(char * send_buf);
  virtual int DealwithRecv(char * recv_buf, int len);
  virtual bool IsOneTimeCnt() {return true;}

  void ChangeFrameQuality(int tag);
  void MoveCamera(int value);
  void RotateCamera(int value);
 private:
  std::string request_buff_;
  std::string start_mark_;
  std::string end_mark_;
  std::string action_values_;

  std::string gl_frame_log_;
  std::string user_activity_log_;

  int type_;
  int value_;
  int sent_tag_;
  Animat * user_obj_ptr_;
  Record * record_obj_ptr_;

  int move_tag_;
  int rotate_tag_;
  int change_quality_tag_;

  char * gl_frame_buf_;
  char * user_activity_buf_;
  int gl_frame_buf_len_;
  int user_activity_buf_len_;
};

#endif	 

/* ---END--- */


