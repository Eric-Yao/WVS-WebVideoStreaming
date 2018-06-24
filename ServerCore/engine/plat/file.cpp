#include "file.h"

int get_file_size(const char *path )
{
	struct stat sbuf;
	if ( stat( path, & sbuf ) == 0 )
		return (int)sbuf.st_size;
	else
		return -1;
}

bool file_read(const char *FileName, int Address, int ReadBufLen, char *ReadBuf)
{
	int fp,bytes_read;
	
	if((fp=open(FileName,O_RDONLY))==-1) 
	{ 
		fprintf(stderr,"PFL_Read Open %s Error:%s\n",FileName,strerror(errno)); 
		return false;
	} 
	lseek(fp, Address, SEEK_SET);
	bytes_read=(int)read(fp,ReadBuf,ReadBufLen);
	if((bytes_read==-1)&&(errno!=-1))
	{ 
		fprintf(stderr,"Read %s Error:%s\n",FileName, strerror(errno)); 
		return false;
	} 
	close(fp); 
	return true;
}



bool file_write(const char *FileName,int Address, int WriteBufLen, char *WriteBuf)
{
	int fp,bytes_read;
	/*char buffer[BUFFER_SIZE];*/
	if((fp=open(FileName,O_WRONLY))==-1) 
	{ 
		fprintf(stderr,"PFL_Write Open %s Error:%s\n",FileName,strerror(errno)); 
		return false;
	} 
	lseek(fp, Address, SEEK_SET);
	bytes_read=(int)write(fp,WriteBuf,WriteBufLen);
	if((bytes_read==-1)&&(errno!=-1))
	{ 
		fprintf(stderr,"Write %s Error:%s\n",FileName, strerror(errno)); 
		return false;
	} 
	close(fp); 
	return true;
	
}

bool file_create(const char *FileName)
{
	int fp;
	if((fp = open(FileName, (O_CREAT | O_TRUNC | O_EXCL | O_RDWR), (S_IRWXU | S_IRWXG | S_IRWXO))) == -1)
	{
		fprintf(stderr,"dFL_Creat  %s Error:%s\n",FileName,strerror(errno));
		//close(fp);
		return false;
	} 
	close(fp); 
	return true;
}


int file_open(const char *FileName,int Type)
{
	int fp=-1;
	/*char buffer[BUFFER_SIZE];*/
	if(DFL_READ == Type)
	{
		if((fp=open(FileName,O_RDONLY))==-1) 
		{ 
			fprintf(stderr,"dFL_Read Open %s Error:%s\n",FileName,strerror(errno)); 
			return -1;
		} 
	}
	else if(DFL_WRITE == Type)
	{
		if((fp=open(FileName,O_WRONLY))==-1) 
		{ 
			fprintf(stderr,"dFL_Write Open %s Error:%s\n",FileName,strerror(errno)); 
			return -1;
		} 
	}
	
	return fp;
}

bool file_close(int fp)
{
	if(-1 != fp)
		close(fp);
	return true;
}


bool file_delet(const char *path)
{
	
	int retCode = -1;
	
	if(NULL == path)
	{
		return false;
	}
	
	retCode = remove(path);
	if(0 == retCode)
	{
		return true;
	}
	
	return false;
}




