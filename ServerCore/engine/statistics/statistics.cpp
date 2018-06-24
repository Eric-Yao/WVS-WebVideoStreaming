#include "statistics.h"

Statistics::Statistics(const char* log_path) {
  store_counter_ = 0;
  gl_frame_log_ = log_path;
  gl_frame_log_.append("/gl_frame_log.txt");
  user_activity_log_ = log_path;
  user_activity_log_.append("/user_activity_log.txt");

  if (file_open(gl_frame_log_.c_str(), DFL_READ) == -1) {
    file_create(gl_frame_log_.c_str());
  } else {
    file_delet(gl_frame_log_.c_str());
    file_create(gl_frame_log_.c_str());
  }

  if (file_open(user_activity_log_.c_str(), DFL_READ) == -1) {
    file_create(user_activity_log_.c_str());
  } else {
    file_delet(user_activity_log_.c_str());
    file_create(user_activity_log_.c_str());
  }

  for (int i = 0; i < 4; ++i) {
    user_activity_info_.move_nos[i] = 1;
    user_activity_info_.rotate_nos[i] = 1;
  }
  for (int i = 0; i < 3; ++i) {
    user_activity_info_.change_quality_nos[i] = 1;
  }

  for (int i = 0; i < 20; ++i) {
     gl_frame_jpeg_info_[i].rendering_nos = 0;
     gl_frame_jpeg_info_[i].sent_nos = 0;
     gl_frame_jpeg_info_[i].total_size = 0;
  }
}

Statistics::~Statistics() {
}

void Statistics::UpdateGlFrameInfo(int quality, float sent_size) {
  //FrameInfo info;
  /*
  if (gl_frame_jpeg_info_[quality] == NULL) {
    FrameInfo * info = new FrameInfo;
    info->rendering_nos = 0;
    info->sent_nos = 0;
    info->total_size = 0;
    gl_frame_jpeg_info_[quality] = info;
  }
  */
  int index = quality/5 -1;
  gl_frame_jpeg_info_[index].rendering_nos ++;
  if (sent_size >= 0) {
    gl_frame_jpeg_info_[index].sent_nos ++;
    gl_frame_jpeg_info_[index].total_size += sent_size;
  }
}

void Statistics::UpdateUserInfo(int move, int rotate, int change) {
  //FrameInfo info;
  //user_activity_info_
  if (move >= 0)
    user_activity_info_.move_nos[move] ++ ;
  if (rotate >= 0)
    user_activity_info_.rotate_nos[rotate] ++;
  if (change >= 0) {
    user_activity_info_.change_quality_nos[change] ++;

  }
}

void Statistics::StoreInfo(char * glframeinfo_buffer, char * useractivity_buffer,  int &len) {
  if (store_counter_ == 0) {
    StoreGlFrameInfo(glframeinfo_buffer, len);
    StoreUserInfo(useractivity_buffer, len);
  }
  store_counter_++;
  if (store_counter_ == 100) {
    StoreGlFrameInfo(glframeinfo_buffer, len);
  } else if (store_counter_ >= 200) {
    StoreUserInfo(useractivity_buffer, len);
    store_counter_ = 1;
  } else {
  }
}

void Statistics::StoreUserInfo( char * useractivity_buffer, int &len) {
  memset(record_buff_, 0x0, kStatisticsBufferSize);
  memset(useractivity_buffer, 0x0, len);
  sprintf(record_buff_, "%d,%d,%d,%d\n%d,%d,%d,%d\n%d,%d,%d\n",
          user_activity_info_.move_nos[0],
          user_activity_info_.move_nos[1],
          user_activity_info_.move_nos[2],
          user_activity_info_.move_nos[3],
          user_activity_info_.rotate_nos[0],
          user_activity_info_.rotate_nos[1],
          user_activity_info_.rotate_nos[2],
          user_activity_info_.rotate_nos[3],
          user_activity_info_.change_quality_nos[0],
          user_activity_info_.change_quality_nos[1],
          user_activity_info_.change_quality_nos[2]);
  record_buff_[(int)strlen(record_buff_)] = '\0';
  user_activity_log_len_  = (int)strlen(record_buff_);
  memcpy(useractivity_buffer, record_buff_, gl_frame_info_log_len_);
  
  //file_write(user_activity_log_.c_str(), 0, (int)strlen(record_buff_), record_buff_);
}

void Statistics::StoreGlFrameInfo(char * glframeinfo_buffer, int &len) {
  int delta = 5;
  int length = 0;
  char temp[100] = {};

  memset(record_buff_, 0x0, kStatisticsBufferSize);
  memset(glframeinfo_buffer, 0x0, len);

  for (int i = 0; ((i + 1) * delta) < 100; i ++) {
    memset(temp, 0x0, 100);
    sprintf(temp, "%d,%d,%d,%f\n", (i + 2) * delta, gl_frame_jpeg_info_[i].rendering_nos,
              gl_frame_jpeg_info_[i].sent_nos,  gl_frame_jpeg_info_[i].total_size/1024);
    memcpy(record_buff_ + length, temp, strlen(temp));
    length += strlen(temp);
  }
  record_buff_[(int)strlen(record_buff_)] = '\0';

  gl_frame_info_log_len_ = (int)strlen(record_buff_);
  memcpy(glframeinfo_buffer, record_buff_, gl_frame_info_log_len_);
  //file_write(gl_frame_log_.c_str(), 0, (int)strlen(record_buff_), record_buff_);
}
