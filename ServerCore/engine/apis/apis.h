
#ifndef APIS_H_
#define APIS_H_


#include "include.h"
#include "plat.h"


void start_init_texture_path(char * path);


void start_with_windows_with_jpg(int argc, char **argv, int width, int height);

void start_by_glx_with_jpg(char* filepath, int width, int height);
void start_by_glx_with_mp4(char* filepath, int width, int height);
void start_by_glx_with_jpg_with_noblock(char* filepath, int width, int height);

#endif
