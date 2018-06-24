#include "mp4write.h"

////////////////////////////////////////////////////////////////////////////
struct VideoInfo {
  int width;
  int height;
  int pixels;
  int is_color;
  int fps;
#if kOpenCV
  IplImage* img;
#endif
  char *raw_image;
  char *flip_buffer;
#if kOpenCV
  CvVideoWriter *writer;
#endif
};

VideoInfo gVideoInfo;

void mp4_write_init(const char * filename, int width, int height){
  gVideoInfo.is_color = 1;
  gVideoInfo.fps     = 10;
  gVideoInfo.width  = width;
  gVideoInfo.height  = height;
  gVideoInfo.pixels = width * height * 3;
#if kOpenCV
  gVideoInfo.writer = NULL;
#endif
  gVideoInfo.raw_image = new char[gVideoInfo.pixels];
  gVideoInfo.flip_buffer = new char[gVideoInfo.pixels];

  // You can change CV_FOURCC('' to CV_FOURCC('X', '2', '6', '4') for x264 output
  // Make sure you compiled ffmpeg with x264 support and OpenCV with x264

  std::string fileName = "/Users/yongyao/YONG/OpenGL/out.mp4";
  //std::string fileName = "/home/yong/out.mp4";
  
#if kOpenCV
  gVideoInfo.writer = cvCreateVideoWriter(fileName.c_str(),
                                          //CV_FOURCC('j', 'p', 'e', 'g'),
                                          //CV_FOURCC('d', 'i', 'v', 'x'),
                                          CV_FOURCC('h', '2', '6', '3'),
                                          //CV_FOURCC_PROMPT,
                                          gVideoInfo.fps,
                                          cvSize(gVideoInfo.width, gVideoInfo.height),
                                          gVideoInfo.is_color);
  if (gVideoInfo.writer == NULL) {
    printf("creating cvWriter failed!\n");
  } else {
    printf("creating cvWriter ok!\n");
  }
#endif
  //gVideoInfo.writer = cvCreateVideoWriter(fileName.c_str(), CV_FOURCC('x','v','i','d'), fps, cvSize(width, height), isColor);
}

void mp4_write_destroy(){
  delete gVideoInfo.raw_image;
  delete gVideoInfo.flip_buffer;
#if kOpenCV
  cvReleaseVideoWriter(&gVideoInfo.writer);
#endif
}

void mp4_write_reset() {
  memset(gVideoInfo.raw_image,0x0,sizeof(char)*gVideoInfo.pixels);
  memset(gVideoInfo.flip_buffer,0x0,sizeof(char)*gVideoInfo.pixels);
  glReadPixels(0, 0, gVideoInfo.width, gVideoInfo.height, GL_RGB, GL_BYTE, gVideoInfo.raw_image);

  for (int x = 0; x < gVideoInfo.width; x++) {
    for (int y = 0; y < gVideoInfo.height; y++) {
      gVideoInfo.flip_buffer[(y*gVideoInfo.height+x)*3] = gVideoInfo.raw_image[((gVideoInfo.width-1-y)*gVideoInfo.height+x)*3];
      gVideoInfo.flip_buffer[(y*gVideoInfo.height+x)*3+1] = gVideoInfo.raw_image[((gVideoInfo.width-1-y)*gVideoInfo.height+x)*3+1];
      gVideoInfo.flip_buffer[(y*gVideoInfo.height+x)*3+2] = gVideoInfo.raw_image[((gVideoInfo.width-1-y)*gVideoInfo.height+x)*3+2];
    }
  }

}

void mp4_write_file () {
#if kOpenCV
  gVideoInfo.img = cvCreateImage(cvSize(gVideoInfo.width, gVideoInfo.height), IPL_DEPTH_8U, 3);
  gVideoInfo.img->imageData = gVideoInfo.flip_buffer;
  cvWriteFrame(gVideoInfo.writer, gVideoInfo.img);      // add the frame to the file
  cvReleaseImage(&gVideoInfo.img);
#endif
}


