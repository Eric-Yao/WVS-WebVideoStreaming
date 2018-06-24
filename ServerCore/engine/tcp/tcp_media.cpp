#include "tcp_media.h"

Media::Media(char * contet_buff, Record* record, pthread_mutex_t mutex)
  : m_ContentBuff(contet_buff), m_BuffMutex(mutex), m_Record(record) {
	m_NoblockServer=-1;
    m_BlockServer=-1;

    m_Counter=0;

}

Media::~Media()
{
	close_socket(m_NoblockServer);
    close_socket(m_BlockServer);

	m_NoblockServer=-1;
    m_BlockServer=-1;

    m_Counter=0;
}

int Media::close_socket(int s)
{ 
	if(s!=-1)
		close(s);	
	return -1;
}


bool Media::format_image_header(char *SendBuff, int contentLen, int *SendBuffLength)
{
	char file_size[10]={0};
	char file_rang[10]={0};

		
	printf(file_size,10,": %d",contentLen);
	printf(file_rang,10,": %d",contentLen-1);

	memset(m_ImageHeader,'\0',1024);

	strcat(m_ImageHeader,"HTTP/1.1 200 OK");
	strcat(m_ImageHeader,"\r\n");
	
	strcat(m_ImageHeader,"Server: Apache/1.3.14(Unix)");
	strcat(m_ImageHeader,"\r\n");
	
	
	strcat(m_ImageHeader,"Connection: close");
	strcat(m_ImageHeader,"\r\n");
	
	strcat(m_ImageHeader,"Content-Type: image/jpeg");
	strcat(m_ImageHeader,"\r\n");
	
	strcat(m_ImageHeader,"Content-Length");
	strcat(m_ImageHeader,file_size);
	strcat(m_ImageHeader,"\r\n");
	
	strcat(m_ImageHeader,"Content-Range: bytes0/");
	strcat(m_ImageHeader,file_rang);
	strcat(m_ImageHeader,"\r\n");
	strcat(m_ImageHeader,"\r\n");

	*SendBuffLength = strlen(m_ImageHeader);
	memcpy(SendBuff,m_ImageHeader, strlen(m_ImageHeader));
	
	return true;
}

bool Media::format_media_header(char *SendBuff, int contentLen, int *SendBuffLength)
{
	char file_size[10]={0};
	char file_rang[10]={0};

		
	printf(file_size,10,": %d",contentLen);
	printf(file_rang,10,": %d",contentLen-1);

	memset(m_ImageHeader,'\0',1024);

	strcat(m_ImageHeader,"HTTP/1.1 200 OK");
	strcat(m_ImageHeader,"\r\n");
	
	strcat(m_ImageHeader,"Server: Apache/1.3.14(Unix)");
	strcat(m_ImageHeader,"\r\n");

	strcat(m_ImageHeader,"Connection: keep-alive");
	strcat(m_ImageHeader,"\r\n");
	
	strcat(m_ImageHeader,"Content-Type: video/mp4");
	strcat(m_ImageHeader,"\r\n");
	
	strcat(m_ImageHeader,"Content-Length");
	strcat(m_ImageHeader,file_size);
	strcat(m_ImageHeader,"\r\n");
	
	strcat(m_ImageHeader,"Content-Range: bytes0/");
	strcat(m_ImageHeader,file_rang);
	strcat(m_ImageHeader,"\r\n");
	strcat(m_ImageHeader,"\r\n");

	*SendBuffLength = strlen(m_ImageHeader);
	memcpy(SendBuff,m_ImageHeader, strlen(m_ImageHeader));
	
	return true;
}

int Media::receive_data(int Socket)
{
	int RecvLen;
	char Buff[2048];
	
    printf("data coming: \n");
    
	memset(Buff, 0x0, 2048);
	RecvLen=recv(Socket,m_ReceiveBuffer,2047,0);
	if(RecvLen > 0)
	{
		Buff[RecvLen]='\0';
		printf("%s\n",m_ReceiveBuffer);
	}
    else if(RecvLen ==0)
    {
        printf("server: recv buf size is 0!\n");
    }
    else
    {
        printf("server: recv error! RecvLen:%d \n",RecvLen);
    }
    
    return RecvLen;
}

int Media::send_jpg_data(int Socket)
{
	int HeaderLen = 0;
    int  FileSize=0;
    int SentLen = 0;
    int ContentLen = 0;

	memset(m_SendBuffer, 0x0, kBufferSize);

#if 0
	memset(m_FileName,0x0,kStringLen);
	sprintf(m_FileName, "/Library/WebServer/Documents/test/images/pic_%d.jpg", m_Counter);
	//printf(m_FileName, "/home/yong/images/pic_%d.jpg", m_Counter);

	FileSize=get_file_size(m_FileName);

	if(FileSize<=0)
		printf("reading file error!\n");
#endif

    format_image_header(m_SendBuffer, FileSize, &HeaderLen);
	//file_read(m_FileName,0, FileSize, m_SendBuffer + HeaderLen);

    pthread_mutex_lock(&m_BuffMutex);
    if (m_Record != NULL)
      ContentLen = m_Record->jpg_file_size();
    else
      ContentLen = get_jpgcontent_size();
    memcpy(m_SendBuffer + HeaderLen, m_ContentBuff, ContentLen);
    pthread_mutex_unlock(&m_BuffMutex);

    //printf("ContentLen:%d\n", ContentLen);
	SentLen=send(Socket, m_SendBuffer, (HeaderLen + ContentLen), 0);

	if(SentLen<0)
		printf("server: sending error!\n");
    else if(SentLen ==0)
    {
        printf("server: sending size is 0!\n");
    }

    m_Counter++;
    if (m_Counter >= 50)
      m_Counter = 0;
    return SentLen;
    
}


int Media::send_media_data(int Socket)
{
	int HeaderLen = 0;
    int  FileSize=0;
    int SentLen = 0;
    int ReadedLen = 0;

	memset(m_SendBuffer, 0x0, kBufferSize);

	memset(m_FileName,0x0,kStringLen);
	sprintf(m_FileName, "/Users/yongyao/YONG/OpenGL/opengl_to_video_sample/opengl-to-video-sample-master/out.mp4");
	
	FileSize=get_file_size(m_FileName);
	
	if(FileSize<=0)
		printf("reading file error!\n");

    format_media_header(m_SendBuffer, FileSize, &HeaderLen);

    while ( ReadedLen < FileSize) {
      if (ReadedLen == 0) {
	    file_read(m_FileName, 0 , 1024, m_SendBuffer + HeaderLen);
        SentLen=send(Socket, m_SendBuffer, HeaderLen + 1024, 0);
       // bSleep(100);
      } else {
        file_read(m_FileName, ReadedLen , 1024, m_SendBuffer);
        SentLen=send(Socket, m_SendBuffer, 1024, 0);
       // bSleep(200);
      }

	  if(SentLen<0) {
	  	//printf("server: sending error!\n");
      } else if(SentLen ==0) {
        printf("server: sending size is 0!\n");
      } else {
        printf("server: sent %d\n", SentLen);
      }

      ReadedLen += 1024;
    }

    return SentLen;
    
}

bool Media::setup_noblock_server(int serverport) {
	sockaddr_in		LocalAddr;
	sockaddr_in		DstAddr;
	int				AddrLen;
	int 			sConnect;
    int             Flag;
	
	AddrLen = sizeof(DstAddr);
	
    printf("setup_no_block_server 1!\n");
	if(-1!=m_NoblockServer)
		printf("block server is already set up!\n");
	
	m_NoblockServer=socket(AF_INET, SOCK_STREAM, 0);
	if(m_NoblockServer==-1)
	{
		printf("server: setting up of block server is error!\n");
		return false;
	}
	
	m_NoblockServerPort=serverport;
	
	LocalAddr.sin_family=AF_INET;
	LocalAddr.sin_port=htons(serverport);
	LocalAddr.sin_addr.s_addr=INADDR_ANY;
	//LocalAddr.sin_addr.s_addr=dSK_Inet_addr("192.168.20.64");
    
    if ((Flag = fcntl(m_NoblockServer, F_SETFL, 0)) < 0) { 
        printf("server: fcntl err 1\n");
        goto createAccept_failed;
    } 
    
    if (fcntl(m_NoblockServer, F_SETFL, Flag | O_NONBLOCK) < 0)
    { 
        printf("server: fcntl err 2\n");
        goto createAccept_failed;
    }
    
	if(bind(m_NoblockServer,(struct sockaddr *)&LocalAddr,sizeof(LocalAddr))<0)
	{	
		printf("server: bind err\n");
		goto createAccept_failed;
	}
    
	if(listen(m_NoblockServer,1000)<0)
	{
		printf("server: listen err\n");
		goto createAccept_failed;
	} else {
      sockaddr_in		DstAddr;
      int				AddrLen;
	  AddrLen 		=	sizeof(DstAddr);

      int sConnect;
      while (1) {
        sConnect=accept(m_NoblockServer,(struct sockaddr *)&DstAddr,(socklen_t*)&AddrLen);
        //printf("<ctListen.c><listen_thread> accept peer coming  \n");

        if(sConnect<0) {
          //printf("accept error! \n");
        }	else {
        receive_data(sConnect);
        bSleep(5);
        send_jpg_data(sConnect);
        bSleep(5);

        sConnect=close_socket(sConnect);
      }
      bSleep(1);
    }
  }
	
    return true;
createAccept_failed:
	close_socket(m_NoblockServer);
	return false;
}

bool Media::setup_block_server(int serverport)
{
	sockaddr_in		LocalAddr;
	sockaddr_in		DstAddr;
	int				AddrLen;
	int 			sConnect;
	
	AddrLen 		=	sizeof(DstAddr);
	
    printf("setup_block_server 1!\n");
	if(-1!=m_BlockServer)
		printf("block server is already set up!\n");
	
	m_BlockServer=socket(AF_INET, SOCK_STREAM, 0);
	if(m_BlockServer==-1)
	{
		printf("setting up of block server is error!\n");
		return false;
	}

    printf("setup_block_server 2\n");


	m_BlockServerPort=serverport;
	
	LocalAddr.sin_family=AF_INET;
	LocalAddr.sin_port=htons(serverport);
	LocalAddr.sin_addr.s_addr=INADDR_ANY;
	//LocalAddr.sin_addr.s_addr=dSK_Inet_addr("192.168.20.64");
    
	if(bind(m_BlockServer,(struct sockaddr *)&LocalAddr,sizeof(LocalAddr))<0)
	{	
		printf("bind err\n");
		goto createAccept_failed;
	}
    
	if(listen(m_BlockServer,1000)<0)
	{
		printf("bind err\n");
		goto createAccept_failed;
	}

    printf("setup_block_server 3\n");

	while(1)
	{
		sConnect=accept(m_BlockServer,(struct sockaddr *)&DstAddr,(socklen_t*)&AddrLen);
		//printf("<ctListen.c><listen_thread> accept peer coming  \n");
		if(sConnect<0)
		{
			printf("accept error! \n");
		}
		else
		{
			receive_data(sConnect);
			send_jpg_data(sConnect);
			sConnect=close_socket(sConnect);
		}		
	}
	return true;
createAccept_failed:
	m_BlockServer=close_socket(m_BlockServer);
	return false;
}

