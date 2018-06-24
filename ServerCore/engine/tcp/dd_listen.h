
#ifndef	_dd_listen_h_
#define	_dd_listen_h_

#include "include.h"
#include "file.h"
#include "plat.h"


#define LISTEN_enable_multiple_tasks (0)

typedef enum _TraceType {
  kRecvString,
  kRecvByte,
  kSendString,
  kSendByte
}TraceType;

class Listen {
 protected:
    
 protected:
    void  TraceBuffer(char *buff, unsigned int len , TraceType Type);
    
 public:
  Listen(void* creator, unsigned int port, unsigned int max_cnt);
  ~Listen();
  void     Disable();
  bool     Enable();
  int      TryAccepte();
  int       socket(){return socket_;}
  unsigned int  port(){return port_;}

 private:
  unsigned int port_;
  unsigned int max_cnt_;
  int  socket_;
  void* creator_;
};


#endif	 






