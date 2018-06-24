#include "tcp_server.h"

TcpServer::TcpServer() {
  listen_ = NULL;
  client_socket_ = -1;
  timeout_.tv_sec = 0;
  timeout_.tv_usec = 3;
  check_cnt_nos_ = 0;
}

TcpServer::~TcpServer() {
  if (listen_ != NULL)
    delete listen_;
}

bool TcpServer::Setup(int port) {
  listen_ = new Listen((void*) this, port, 100);
  return listen_->Enable();
}

void TcpServer::Run(char * recv_buf, char * send_buf, unsigned int & len) {
  /*if (client_socket_ < 0) {
    client_socket_ = listen_->TryAccepte();
    if (client_socket_ >= 0) {
      FD_SET(client_socket_, &read_set_);
      FD_SET(client_socket_, &write_set_);
      printf("accepted\n");
    } else {
      return;
    }
  }
  */

  client_socket_ = listen_->TryAccepte();
  if (client_socket_ < 0) return;

  FD_SET(client_socket_, &read_set_);
  FD_SET(client_socket_, &write_set_);
  //printf("incoming 1\n");

  select(client_socket_ + 1, &read_set_, &write_set_, &err_set_, &timeout_);
  //if (false ==
  bool tag =CheckCnt(FD_ISSET(client_socket_, &read_set_) > 0,
                     FD_ISSET(client_socket_, &write_set_ ) > 0,
                     recv_buf, send_buf, len);
  if (tag == false) {
    close(client_socket_);
    client_socket_ = -1;
  } else {
    //some other functionalities will be added later on
    //now let it have the same with the above case;
    close(client_socket_);
    client_socket_ = -1;
  }
  
  FD_ZERO(&read_set_);
  FD_ZERO(&write_set_);
  FD_ZERO(&err_set_);

}

bool TcpServer::CheckCnt(bool readable, bool writeable, char * recv_buf, char * send_buf, unsigned int & len) {
  if (readable == false && writeable == false) {
      printf("<<<<<<<<<<<<<<(readable == false && writeable == false)!\n");
//    check_cnt_nos_ ++;
//    if (check_cnt_nos_ >0) {
//      check_cnt_nos_ = 0;
//    }
    return false;
  }

  memset(recv_buf, 0x0, len);
  memset(send_buf, 0x0, len);

  if (readable == true) {
    int recv_len = recv(client_socket_, recv_buf, len, 0);
    if(recv_len<=0) {
      printf("<<<<<<<<<<<<<<<recv buffer error!\n");
      return false;
    } else {
      recv_buf[recv_len]='\0';
      DealwithRecv(recv_buf, recv_len);
    }
  }

  bSleep(1);
  if (writeable == true) {
    int buf_len_for_send = PrepareforSend(send_buf); // can become call back function.
    if(buf_len_for_send>0) {
      //  printf("buf_len_for_send: %d!\n %s \n", buf_len_for_send, send_buf);
      int send_len = send(client_socket_, send_buf, buf_len_for_send, 0);
      if (send_len<=0) {
        printf("send buffer error!\n");
        return false;
      } else {
      }
    } else {
      return false;
    }
    bSleep(1);
  }
  return true;
}

