#include "tcp_streaming.h"

Streaming::Streaming() : TcpServer() {
  counter_ = 0;
  sent_max_size_ = 0;
  sent_min_size_ = 1024*1024*500;
  sent_size_ = -1.0;
}

Streaming::~Streaming() {
}

void Streaming::SetContentBuff(char * content_buff, unsigned int len) {
  content_buff_ = content_buff;
  content_buff_len_ = len;
}


int Streaming::PrepareforSend(char * send_buf) {
  int size = send_jpg_data(send_buf);
  
  if (sent_max_size_ < size)
    sent_max_size_ = size;

  if (sent_min_size_ > size)
    sent_min_size_ = size;

  sent_size_ = size;
  sent_size_ /=1024.0f;

  return size;
}

int Streaming::DealwithRecv(char * recv_buf, int len) {
  return 0;
}

float Streaming::sent_size() {
  float size = sent_size_;
  sent_size_ = -1.0;
  return size;
}

bool Streaming::format_image_header(char *SendBuff, int contentLen, int *SendBuffLength) {
	char file_size[10]={0};
	char file_rang[10]={0};
		
	sprintf(file_size,": %d",contentLen);
	sprintf(file_rang,": %d",contentLen-1);
    

	memset(image_header_,'\0',1024);

	strcat(image_header_,"HTTP/1.1 200 OK");
	strcat(image_header_,"\r\n");
	
	strcat(image_header_,"Server: Apache/1.3.14(Unix)");
	strcat(image_header_,"\r\n");
	
	
	strcat(image_header_,"Connection: close");
	strcat(image_header_,"\r\n");
	
	strcat(image_header_,"Content-Type: image/jpeg");
	strcat(image_header_,"\r\n");
	
	strcat(image_header_,"Content-Length: ");
	strcat(image_header_,file_size);
	strcat(image_header_,"\r\n");
	
	strcat(image_header_,"Content-Range: bytes0/");
	strcat(image_header_,file_rang);
	strcat(image_header_,"\r\n");
	strcat(image_header_,"\r\n");

	*SendBuffLength = (int) strlen(image_header_);
	memcpy(SendBuff,image_header_, strlen(image_header_));
	
	return true;
}

int Streaming::send_jpg_data(char * send_buf) {
	int HeaderLen = 0;

    format_image_header(send_buf, content_buff_len_, &HeaderLen);
    memcpy(send_buf + HeaderLen, content_buff_, content_buff_len_);

    //printf(" HeaderLen (%d) + FileSize(%d): %d!\n",  HeaderLen, content_buff_len_, HeaderLen + content_buff_len_);

    return (HeaderLen + content_buff_len_);
    
}
