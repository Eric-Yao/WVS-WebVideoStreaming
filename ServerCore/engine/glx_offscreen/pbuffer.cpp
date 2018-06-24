
#include "pbuffer.h"

PBuffer::PBuffer(int width, int height) {
  initialized=false;

  //printf("PBuffer::PBuffer 1 \n");
  this->width=width;
  this->height=height;

  //printf("PBuffer::PBuffer 2\n");

   dpy = XOpenDisplay(NULL);//
 // dpy = glXGetCurrentDisplay();

  if(dpy == NULL) printf("invlide display\n");
  //dpy = XOpenDisplay(0);

  //printf("PBuffer::PBuffer 3\n");
  scrnum = DefaultScreen( dpy );

  //printf("PBuffer::PBuffer 4\n");
  FBRC = glXGetCurrentContext();

  //printf("PBuffer::PBuffer 5 \n");
  FBDC = glXGetCurrentDrawable();

  //printf("PBuffer::PBuffer\n");
  Create();
}

PBuffer::~PBuffer() {
  Destroy();
}

void PBuffer::Setup() {
  // make sure the pbuffer has been initialized
  if (!initialized) {
    throw std::runtime_error("pbuffer is not initialized");
  }
  // make sure we haven't lost our pbuffer due to a display mode change

  glViewport(0, 0, width, height);

  // resize view port. generally you'll want to set this to the
  // size of your pbuffer so that you render to the entire pbuffer
  // but there are cases where you might want to render to just a
  // sub-region of the pbuffer.
  glXMakeContextCurrent(dpy, PBDC, PBDC, PBRC);


//glClearColor(0.81f, 0.81f, 0.81f, 1.0f);		// This Will Clear The Background Color To Black
glClearColor(0.0f, 1.0f, 1.0f, 1.0f);		// This Will Clear The Background Color To Black
  glClearDepth(1.0);				// Enables Clearing Of The Depth Buffer
  glDepthFunc(GL_LESS);				// The Type Of Depth Test To Do
  glEnable(GL_TEXTURE_2D);			// Enable Texture Mapping  
  glEnable(GL_DEPTH_TEST);			// Enables Depth Testing
  glShadeModel(GL_SMOOTH);			// Enables Smooth Color Shading
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();				    // Reset The Projection Matrix
  glMatrixMode(GL_MODELVIEW);

  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  GLUPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);	// Calculate The Aspect Ratio Of The Window
  glMatrixMode(GL_MODELVIEW);
}

void PBuffer::Start() {
  glDrawBuffer(GL_FRONT);
 /// printf("PBuffer::Start\n");
 // glClearColor(1, 1.0, 1.0, 1.0);
 // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void PBuffer::Finish() {
  glReadBuffer(GL_FRONT);
 //   printf("PBuffer::Finish\n");

}

void PBuffer::Release() {
  // make sure the pbuffer has been initialized
  if ( !initialized ) {
    throw std::runtime_error("pbuffer is not initialized");
  }
  // make sure we haven't lost our pbuffer due to a display mode change
  glXMakeContextCurrent(dpy, FBDC, FBDC, FBRC);
}

void PBuffer::Create() {
  if(initialized) {
    Destroy();
  }

  if (dpy == NULL) {
    throw std::runtime_error("unable to get device context");
  }

  if (0) {//!FBDC) {
    throw std::runtime_error("unable to get render context");
  }
  // define the minimum pixel format requirements we will need for our pbuffer
  // a pbuffer is just like a frame buffer, it can have a depth buffer associated
  // with it and it can be double buffered.
   /*int attr[] =
    {
        GLX_RENDER_TYPE, GLX_RGBA_BIT,
        GLX_DRAWABLE_TYPE, GLX_PBUFFER_BIT | GLX_WINDOW_BIT,
        GLX_DOUBLEBUFFER, False,
        GLX_RED_SIZE, 1,
        GLX_GREEN_SIZE, 1,
        GLX_BLUE_SIZE, 1,
        GLX_ALPHA_SIZE, 1,
        GLX_DEPTH_SIZE, 1,
        0 // zero terminates the list
    };*/

  int attrib[] = {
    GLX_DOUBLEBUFFER,  false,
    GLX_RED_SIZE,      8,
    GLX_GREEN_SIZE,    8,
    GLX_BLUE_SIZE,     8,
    GLX_ALPHA_SIZE,    8,
    GLX_STENCIL_SIZE,  1,
    GLX_DEPTH_SIZE,    24,
    GLX_RENDER_TYPE,   GLX_RGBA_BIT,
    GLX_DRAWABLE_TYPE, GLX_PBUFFER_BIT | GLX_WINDOW_BIT,
    None
  };

  int PBattrib[] = {
    GLX_PBUFFER_WIDTH,   width,
    GLX_PBUFFER_HEIGHT,  height,
    GLX_LARGEST_PBUFFER, false,
    None
  };

  // choose a pixel format that meets our minimum requirements
  int count = 0;
  //GLXFBConfigSGIX *config=
  //  glXChooseFBConfigSGIX(dpy, scrnum, attrib, &count);

  GLXFBConfig *config= glXChooseFBConfig(dpy, scrnum, attrib, &count);

  if(config == NULL || count == 0) {
    throw std::runtime_error("no matching pbuffer pixel format found");
  }

  // allocate the pbuffer
  //PBDC=glXCreateGLXPbufferSGIX(dpy, config[0], width, height, PBattrib);
  //PBRC=glXCreateContextWithConfigSGIX(dpy, config[0], GLX_RGBA_TYPE_SGIX, FBRC, true);

  PBDC=glXCreatePbuffer(dpy, config[0], PBattrib);
  PBRC=glXCreateNewContext(dpy, config[0], GLX_RGBA_TYPE, FBRC, true);
  printf("Create pbuffer done\n");

  XFree(config);
  initialized=true;
}

void PBuffer::Destroy() {
  // make sure the pbuffer has been initialized
  if ( !initialized ) {
    throw std::runtime_error("pbuffer is not initialized");
  }

  Release();
  glXDestroyContext(dpy, PBRC);
  glXDestroyPbuffer(dpy, PBDC);
  initialized = false;
}



