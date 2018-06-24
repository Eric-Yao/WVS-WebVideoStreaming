
#ifndef STATISTICS_H_
#define STATISTICS_H_

#include "plat.h"
#include "file.h"

#define kStatisticsBufferSize (1024*5)

class Statistics {
 public:
  Statistics(const char* log_path);
  ~Statistics();
  void UpdateGlFrameInfo(int quality, float sent_size);
  void UpdateUserInfo(int move, int rotate, int change);
  void StoreInfo(char * glframeinfo_buffer, char * useractivity_buffer, int &len);
  int gl_frame_info_log_len() {return gl_frame_info_log_len_;}
  int user_activity_log_len() {return user_activity_log_len_;}
 private:
  void StoreUserInfo( char * useractivity_buffer, int &len);
  void StoreGlFrameInfo(char * glframeinfo_buffer, int &len);
 private:
  struct FrameInfo {
    int width;
    int height;
    int rendering_nos;
    int sent_nos;
    int average_size;
    float total_size;
  };
  struct ActivityInfo {
    int move_nos[4]; //forward, back, left, right
    int rotate_nos[4]; //up, down, left, right
    int change_quality_nos[3]; //rest, increase, decrease
  };
  //typedef std::map<int, FrameInfo*> GlFrameJpeg;
  FrameInfo gl_frame_jpeg_info_[20];
  ActivityInfo user_activity_info_;
  std::string gl_frame_log_;
  std::string user_activity_log_;
  char record_buff_[kStatisticsBufferSize];
  int store_counter_;

  int gl_frame_info_log_len_;
  int user_activity_log_len_;
};

#endif