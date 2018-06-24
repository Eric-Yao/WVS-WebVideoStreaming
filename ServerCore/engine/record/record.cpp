

#include "record.h"
#include "jpegwrite.h"
#include "mp4write.h"

Record::Record(int width, int height, const char * filepath,
              char * content_buffer, int buffer_len, pthread_mutex_t buffmutex) {
  image_width_ = width;
  image_height_ = height;
  picture_path_ = filepath;
  content_buffer_ = content_buffer;
  buffer_len_ = buffer_len;
  buff_mutex_ = buffmutex;

  jpg_file_size_ = 0;
  jpeg_quality_ = 40;

  count_tag_ = 0;
}

Record::~Record() {
  jpeg_write_destroy();
  mp4_write_destroy();
}

void Record::InitJpeg() {
  jpeg_write_init(image_width_, image_height_);
}

void Record::InitMp4() {
  mp4_write_init("/Users/yongyao/YONG/OpenGL/out.mp4", image_width_, image_height_);
}

void Record::WriteJpeg() {

  if(count_tag_ >= 100)
   count_tag_ = 0;

 // printf("Record::Write 1\n");
  memset(picture_name_, 0x0, 20);
  sprintf(picture_name_, "pic_%d.jpg", count_tag_++);

  full_picture_name_ = picture_path_;
  full_picture_name_.append(picture_name_);
  pthread_mutex_lock(&buff_mutex_);
  jpeg_write_reset();
  jpg_file_size_ = jpeg_write_buffer(NULL, jpeg_quality_, content_buffer_, buffer_len_);
  pthread_mutex_unlock(&buff_mutex_);
  return;
}

void Record::WriteMp4File() {

 // printf("Record::Write 1\n");
  //memset(picture_name_, 0x0, 20);
  //sprintf(picture_name_, "pic_%d.jpg", count_tag_++);

  //full_picture_name_ = picture_path_;
  //full_picture_name_.append(picture_name_);

  mp4_write_reset();

  pthread_mutex_lock(&buff_mutex_);
  mp4_write_file();
  pthread_mutex_unlock(&buff_mutex_);
  return;
}

void Record::SetJpegQuality(int tag) {
  if (tag == 0) {
    jpeg_quality_ = 50;
  } else if (tag == 1) {
    jpeg_quality_ += 5;
    if (jpeg_quality_ > 100)
      jpeg_quality_ = 100;
  } else {
    jpeg_quality_ -= 5;
    if (jpeg_quality_ < 10)
      jpeg_quality_ = 10;
  }
}

