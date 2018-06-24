#include "dd_listen.h"

Listen::Listen(void* creator, unsigned int port, unsigned int max_cnt) {
  max_cnt_ = max_cnt;
  socket_ = -1;
  port_ = port;
}

Listen::~Listen() {
  Disable();
}

void Listen::Disable() {
  if(socket_!=-1) {
    close(socket_);
    socket_=-1;
  }
}


bool Listen::Enable() {
  sockaddr_in	local_addr;
  int			flag;
        
  if(socket_!=-1)
    return false;
        
  socket_ = ::socket(AF_INET, SOCK_STREAM, 0);
  if(socket_ == -1)
    return false;
        
  local_addr.sin_family = AF_INET;
  local_addr.sin_port = htons(port_);
  local_addr.sin_addr.s_addr = INADDR_ANY;//btSK_inet_addr(ListenObj_t->m_IP);
        //LocalAddr.sin_addr.s_addr=dSK_Inet_addr("192.168.20.64");
        
  if ((flag = fcntl(socket_, F_SETFL, 0)) < 0) {
    printf("server: fcntl err 1\n");
    Disable();
    return false;
  }
    
  if (fcntl(socket_, F_SETFL, flag | O_NONBLOCK) < 0) {
    printf("server: fcntl err 2\n");
    Disable();
    return false;
  }
    
  if(bind(socket_,(struct sockaddr *)&local_addr,sizeof(local_addr))<0) {
    printf("server: bind err\n");
	Disable();
    return false;
  }
    
  if(listen(socket_,1000)<0)	{
	printf("server: listen err\n");
	Disable();
    return false;
  }
  return true;
}
    
int Listen::TryAccepte() {
  int	addr_len, cnt, flag;
  sockaddr_in	dst_addr;

  addr_len 		=	sizeof(dst_addr);

  if (socket_ == -1)
    return -1;
        
  cnt = accept(socket_, (struct sockaddr *)&dst_addr, (socklen_t*)&addr_len);
  if(cnt<0) {
    return -1;
  } else {

  #if 0
    if ((flag = fcntl(cnt, F_SETFL, 0)) < 0) {
      printf("bSKFcntlB() error 1!");
      close(cnt);
      return false;
    }

   if (fcntl(cnt, F_SETFL, flag | O_NONBLOCK) < 0) {
      printf("bSKFcntlB() error 2!");
      close(cnt);
      return false;
    }

    #endif
  }
  return cnt;
}

