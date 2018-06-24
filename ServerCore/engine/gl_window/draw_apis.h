
#ifndef _GL_FUNCS_
#define _GL_FUNCS_

#include "plat.h"
#include "draw.h"
#include "jpegwrite.h"

int get_jpgcontent_size();
Draw * setup_gl_environment(int argc, char ** argv, const char* filepath, int width, int height, char* buffer, int len, pthread_mutex_t buffmutex);
void destroy_gl_environment();

#endif