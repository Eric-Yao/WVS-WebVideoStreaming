#include "tcp_streaming.h"

Image::Image() : TcpServer() {
  counter_ = 0;
  sent_nos_ = 0;
  sent_max_size_ = 0;
  sent_min_size_ = 1024*1024*500;
  sent_total_size_ = 0;
}

Image::~Image() {
}

void Image::SetContentBuff(char * content_buff, unsigned int len) {
  content_buff_ = content_buff;
  content_buff_len_ = len;
}


int Image::PrepareforSend(char * send_buf) {
  int size = send_jpg_data(send_buf);

  sent_nos_ ++;
  sent_total_size_ += size;
  
  if (sent_max_size_ < size)
    sent_max_size_ = size;

  if (sent_min_size_ > size)
    sent_min_size_ = size;

  return size;
}

int Image::DealwithRecv(char * recv_buf, int len) {
  return 0;
}


bool Image::format_media_header(char *SendBuff, int contentLen, int *SendBuffLength)
{
	char file_size[10]={0};
	char file_rang[10]={0};

    sprintf(file_size,": %d",contentLen);
	sprintf(file_rang,": %d",contentLen-1);

	memset(image_header_,'\0',1024);

	strcat(image_header_,"HTTP/1.1 200 OK");
	strcat(image_header_,"\r\n");
	
	strcat(image_header_,"Server: Apache/1.3.14(Unix)");
	strcat(image_header_,"\r\n");

	strcat(image_header_,"Connection: keep-alive");
	strcat(image_header_,"\r\n");
	
	strcat(image_header_,"Content-Type: video/mp4");
	strcat(image_header_,"\r\n");
	
	strcat(image_header_,"Content-Length");
	strcat(image_header_,file_size);
	strcat(image_header_,"\r\n");
	
	strcat(image_header_,"Content-Range: bytes0/");
	strcat(image_header_,file_rang);
	strcat(image_header_,"\r\n");
	strcat(image_header_,"\r\n");

	*SendBuffLength = strlen(image_header_);
	memcpy(SendBuff,image_header_, strlen(image_header_));
	
	return true;
}


int Image::send_media_data(char * send_buf) {
	int HeaderLen = 0;
    int  FileSize=0;
    int SentLen = 0;
    int ReadedLen = 0;


	memset(file_name_,0x0,kStringLen);
	sprintf(file_name_, "/Users/yongyao/YONG/PHP/test5.mp4");
	
	FileSize=get_file_size(file_name_);
	
	if(FileSize<=0)
		printf("reading file error!\n");

    format_media_header(send_buf, FileSize, &HeaderLen);

    if ( counter_ < FileSize) {
      if (counter_ == 0) {
	    file_read(file_name_, 0 , 1024, send_buf + HeaderLen);
        SentLen = 1024 + HeaderLen;
      } else {
        file_read(file_name_, counter_ , 1024, send_buf);
        SentLen = 1024;
      }
    } else {
      SentLen = -1;
    }
    counter_ += 1024;

    return SentLen;
    
}
