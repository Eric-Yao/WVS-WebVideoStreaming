
#ifndef JPEGMEM_H_
#define JPEGMEM_H_

#include "include.h"
#include "plat.h"
#include "file.h"


void jpeg_write_init(int width, int height);
void jpeg_write_destroy();
void jpeg_write_reset();
void jpeg_write_file (char * filename, int quality);
int jpeg_write_buffer (const char * filename, int quality, char* content, int content_len);
#endif
