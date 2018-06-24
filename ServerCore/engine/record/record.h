
#ifndef RECORD_H_
#define RECORD_H_

#include "plat.h"

class Record {
 public:
  Record(int width, int height, const char * filepath,
        char * content_buffer, int buffer_len, pthread_mutex_t buffmutex);
  ~Record();
  void InitJpeg();
  void InitMp4();
  void WriteJpeg();
  void WriteMp4File();
  void SetJpegQuality(int tag);

  int  jpg_file_size() {return jpg_file_size_;}
  int  jpg_quality() {return jpeg_quality_;}

 private:
  char picture_name_[20];
  char *content_buffer_;
  int count_tag_;

  int buffer_len_;
  int image_width_ ;		/* Number of columns in image */
  int image_height_ ;	/* Number of rows in image */
  std::string full_picture_name_;
  std::string picture_path_;

  int jpg_file_size_;
  int jpeg_quality_;
  pthread_mutex_t buff_mutex_;
};

#endif