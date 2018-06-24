
#ifndef PBUFFER_H_
#define PBUFFER_H_

#include "include.h"
#include "plat.h"
#include "BmpImage.h"
#include "PrecisionTimer.h"
#include "jpegwrite.h"

class PBuffer {
 public:
  PBuffer(int width, int height);
  virtual ~PBuffer();
  void Setup();
  void Start();
  void Finish();
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

  Display     *dpy;
  int     scrnum;
  GLXContext  PBRC;
  GLXPbuffer  PBDC;

  GLXContext  FBRC;
  GLXDrawable FBDC;
};


#endif
