
#ifndef MP4WRITE_H_
#define MP4WRITE_H_

#include "include.h"
#include "plat.h"
#include "file.h"


void mp4_write_init(const char * filename, int width, int height);
void mp4_write_destroy();
void mp4_write_reset();
void mp4_write_file();

#endif
