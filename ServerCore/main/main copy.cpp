#ifdef WIN32
#include <GL/wglew.h>
#endif

#ifdef GLX
#ifdef NOGLEW
#include <GL/glx.h>
#else
#include <GL/glxew.h>
#endif
#endif

class PBuffer
{
public:
    PBuffer(int width, int height);
    virtual ~PBuffer();

    void Use();
    void Release();

    int const get_width() const { return width; }
    int const get_height() const { return height; }

private:
    void Create();
    void Destroy();

protected:
    int width;
    int height;

    bool initialized;

#ifdef WIN32
    HPBUFFERARB hPB;
    HGLRC       hPBRC;
    HDC     hPBDC;

    HDC     hGLDC;
    HGLRC       hGLRC;
#endif

#ifdef GLX
    Display     *dpy;
    int     scrnum;
    GLXContext  PBRC;
    GLXPbuffer  PBDC;

    GLXContext  FBRC;
    GLXDrawable FBDC;
#endif
};



#include <stdexcept>
#include <GL/glew.h>
#include "pbuffer.h"

using namespace std;

#ifdef WIN32

PBuffer::PBuffer(int width, int height)
{
    initialized=false;

    this->width=width;
    this->height=height;

    hGLDC = wglGetCurrentDC();
    hGLRC = wglGetCurrentContext();

    Create();
}

PBuffer::~PBuffer()
{
    Destroy();
}

void PBuffer::Use()
{
    // make sure the pbuffer has been initialized
    if (!initialized)
    {
        throw runtime_error("pbuffer is not initialized");
    }
    // make sure we haven't lost our pbuffer due to a display mode change

    int flag = 0;
    wglQueryPbufferARB(hPB, WGL_PBUFFER_LOST_ARB, &flag);
    if (flag)
    {
        throw runtime_error("pbuffer became invalid");
    }

    wglMakeCurrent(hPBDC, hPBRC);

    glViewport(0, 0, width, height);

    glDrawBuffer(GL_FRONT);
        glReadBuffer(GL_FRONT);
}

void PBuffer::Release()
{
    // make sure the pbuffer has been initialized
    if ( !initialized )
    {
        throw runtime_error("pbuffer is not initialized");
    }
    // make sure we haven't lost our pbuffer due to a display mode change
    int flag = 0;
    wglQueryPbufferARB(hPB, WGL_PBUFFER_LOST_ARB, &flag);
    if (flag)
    {
        throw runtime_error("pbuffer became invalid");
    }

    wglMakeCurrent(hGLDC, hGLRC);
}

void PBuffer::Create()
{
    if(initialized)
    {
        Destroy();
    }

    if (hGLDC == NULL)
    {
        throw runtime_error("unable to get device context");
    }
    if (hGLRC == NULL)
    {
        throw runtime_error("unable to get render context");
    }
    // define the minimum pixel format requirements we will need for our pbuffer
    // a pbuffer is just like a frame buffer, it can have a depth buffer associated
    // with it and it can be double buffered.
    int attr[] =
    {
        WGL_SUPPORT_OPENGL_ARB, TRUE, // pbuffer will be used with gl
        WGL_DRAW_TO_PBUFFER_ARB, TRUE, // enable render to pbuffer
        WGL_RED_BITS_ARB, 16, // at least 8 bits for RED channel
        WGL_GREEN_BITS_ARB, 16, // at least 8 bits for GREEN channel
        WGL_BLUE_BITS_ARB, 16, // at least 8 bits for BLUE channel
        WGL_ALPHA_BITS_ARB, 16, // at least 8 bits for ALPHA channel
        WGL_DEPTH_BITS_ARB, 24, // at least 24 bits for depth buffer
        WGL_DOUBLE_BUFFER_ARB, FALSE, // we dont require double buffering
        0 // zero terminates the list
    };

    // choose a pixel format that meets our minimum requirements
    unsigned int count = 0;
    int pixelFormat;
    wglChoosePixelFormatARB(hGLDC,(const int*)attr, NULL, 1,&pixelFormat,&count);
    if(count == 0)
    {
        throw runtime_error("no matching pbuffer pixel format found");
    }

    int attribs[]={0,0};

    // allocate the pbuffer
    hPB = wglCreatePbufferARB(hGLDC, pixelFormat, width, height, attribs);
    hPBDC = wglGetPbufferDCARB(hPB);
    hPBRC = wglCreateContext(hPBDC);

    wglShareLists(hGLRC, hPBRC);

    initialized=true;
}

void PBuffer::Destroy()
{
    // make sure the pbuffer has been initialized
    if ( !initialized )
    {
        throw runtime_error("pbuffer is not initialized");
    }

    Release();

    wglDeleteContext(hPBRC);
    wglReleasePbufferDCARB(hPB, hPBDC);
    wglDestroyPbufferARB(hPB);

    initialized = false;
}
#endif

#ifdef GLX
PBuffer::PBuffer(int width, int height)
{
    initialized=false;

    this->width=width;
        this->height=height;

    dpy = glXGetCurrentDisplay();
    scrnum = DefaultScreen( dpy );
    FBRC = glXGetCurrentContext();
    FBDC = glXGetCurrentDrawable();

    Create();
}

PBuffer::~PBuffer()
{
    Destroy();
}

void PBuffer::Use()
{
    // make sure the pbuffer has been initialized
    if (!initialized)
    {
        throw runtime_error("pbuffer is not initialized");
    }
    // make sure we haven't lost our pbuffer due to a display mode change

    // resize view port. generally you'll want to set this to the
    // size of your pbuffer so that you render to the entire pbuffer
    // but there are cases where you might want to render to just a
    // sub-region of the pbuffer.
    glXMakeContextCurrent(dpy, PBDC, PBDC, PBRC);

    glViewport(0, 0, width, height);

    glDrawBuffer(GL_FRONT);
    glReadBuffer(GL_FRONT);
}

void PBuffer::Release()
{
    // make sure the pbuffer has been initialized
    if ( !initialized )
    {
        throw runtime_error("pbuffer is not initialized");
    }
    // make sure we haven't lost our pbuffer due to a display mode change

    glXMakeContextCurrent(dpy, FBDC, FBDC, FBRC);
}

void PBuffer::Create()
{
    if(initialized)
    {
        Destroy();
    }

    if (dpy == NULL)
    {
        throw runtime_error("unable to get device context");
    }
    if (!FBDC)
    {
        throw runtime_error("unable to get render context");
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

    int attrib[] =
    {
        GLX_DOUBLEBUFFER,  False,
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

    int PBattrib[] =
    {
        GLX_PBUFFER_WIDTH,   width,
        GLX_PBUFFER_HEIGHT,  height,
        GLX_LARGEST_PBUFFER, False,
    None
    };

    // choose a pixel format that meets our minimum requirements
    int count = 0;
    //GLXFBConfigSGIX *config=
    //  glXChooseFBConfigSGIX(dpy, scrnum, attrib, &count);

    GLXFBConfig *config=
        glXChooseFBConfig(dpy, scrnum, attrib, &count);

    if(config == NULL || count == 0)
    {
        throw runtime_error("no matching pbuffer pixel format found");
    }

    // allocate the pbuffer
    //PBDC=glXCreateGLXPbufferSGIX(dpy, config[0], width, height, PBattrib);
    //PBRC=glXCreateContextWithConfigSGIX(dpy, config[0], GLX_RGBA_TYPE_SGIX, FBRC, true);

    PBDC=glXCreatePbuffer(dpy, config[0], PBattrib);
    PBRC=glXCreateNewContext(dpy, config[0], GLX_RGBA_TYPE, FBRC, true);

    XFree(config);

    initialized=true;
}

void PBuffer::Destroy()
{
    // make sure the pbuffer has been initialized
    if ( !initialized )
    {
        throw runtime_error("pbuffer is not initialized");
    }

    Release();

    glXDestroyContext(dpy, PBRC);
    glXDestroyPbuffer(dpy, PBDC);

    initialized = false;
}

#endif
