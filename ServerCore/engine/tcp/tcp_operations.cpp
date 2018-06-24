#include "tcp_operations.h"

Operations::Operations(const char* log_path) : TcpServer() {
  start_mark_ = "start=start";
  end_mark_ = "end=end";
  user_obj_ptr_ = NULL;
  type_ = -1;
  value_ = -1;

  move_tag_ = -1;
  rotate_tag_ = -1;
  change_quality_tag_ = -1;
  sent_tag_ = -1;

  gl_frame_log_ = log_path;
  gl_frame_log_.append("/gl_frame_log.txt");
  user_activity_log_ = log_path;
  user_activity_log_.append("/user_activity_log.txt");
}

Operations::~Operations() {
}

void Operations::SetStatistics(char *buf_a, char *buf_b, int len_a, int len_b ) {
  gl_frame_buf_ = buf_a;
  user_activity_buf_ = buf_b;
  gl_frame_buf_len_ = len_a;
  user_activity_buf_len_ = len_b;
}

void Operations::RegisterEnvironment(Animat* userobj, Record* recordobj) {
  user_obj_ptr_ = userobj;
  record_obj_ptr_ = recordobj;
}

int Operations::PrepareforSend(char * send_buf) {
  if (sent_tag_ == -1)
    return -1;

  int len;
  if (sent_tag_ == 1) {
    //len = get_file_size(gl_frame_log_.c_str());
    //file_read(gl_frame_log_.c_str(), 0, len, send_buf);
    len = gl_frame_buf_len_;
    memcpy(send_buf, gl_frame_buf_, gl_frame_buf_len_);
  } else {
    //len = get_file_size(user_activity_log_.c_str());
    //file_read(user_activity_log_.c_str(), 0, len, send_buf);
    len = user_activity_buf_len_;
    memcpy(send_buf, user_activity_buf_, user_activity_buf_len_);
  }
  return len;
}

int Operations::DealwithRecv(char * recv_buf, int len) {
  request_buff_ = recv_buf;

  std::size_t start_pos = request_buff_.find(start_mark_) + start_mark_.length();
  std::size_t end_pos = request_buff_.find(end_mark_);
  std::size_t length = end_pos  - start_pos;

  action_values_ = request_buff_.substr(start_pos,length);

  std::string find;
  find = action_values_.at(action_values_.find("=") + 1);
  type_ = atoi(find.c_str());
  find = action_values_.at(action_values_.rfind("=") + 1);
  value_ = atoi(find.c_str());

  printf("%s, type: %d; value: %d \n", action_values_.c_str(), type_, value_);
  sent_tag_ = -1;

  if (user_obj_ptr_ == NULL) {
    type_ = -1;
    value_ = -1;
    return 0;
  }

  if (type_ == 0) {
    ChangeFrameQuality(value_);
  } else if (type_ == 1) {
    MoveCamera(value_);
  } else if (type_ == 2) {
    RotateCamera(value_);
  } else if (type_ == 3) {
    sent_tag_ = value_;
  } else {

  }

  type_ = -1;
  value_ = -1;
  return 0;
}

void Operations::ChangeFrameQuality(int tag) {
  record_obj_ptr_->SetJpegQuality(tag);
  change_quality_tag_ = tag;
}

void Operations::MoveCamera(int value) {
  switch (value) {
    case 1:
    user_obj_ptr_->MoveHorizon(1, 0);
    move_tag_ = kMoveForward;
    break;
  case 2:
    user_obj_ptr_->MoveHorizon(-1, 0);
    move_tag_ = kMoveBack;
    break;
  case 3:
    user_obj_ptr_->MoveHorizon(0, -1);
    move_tag_ = kMoveLeft;
    break;
  case 4:
    user_obj_ptr_->MoveHorizon(0, 1);
    move_tag_ = kMoveRight;
    break;
  default:
    break;
 }
}

void Operations::RotateCamera(int value) {
  switch (value) {
    case 1:
    user_obj_ptr_->RotateVertical(1.0);
    rotate_tag_ = kRotateUp;
    break;
  case 2:
    user_obj_ptr_->RotateVertical(-1.0);
    rotate_tag_ = kRotateDown;
    break;
  case 3:
    user_obj_ptr_->RotateHorizon(-1.0);
    rotate_tag_ = kRotateLeft;
    break;
  case 4:
    user_obj_ptr_->RotateHorizon(1.0);
    rotate_tag_ = kRotateRight;
    break;
  default:
    break;
  }
}

int Operations::move_tag() {
  int tag = move_tag_;
  move_tag_ = -1;
  return tag;
}

int Operations::rotate_tag() {
  int tag = rotate_tag_;
  rotate_tag_ = -1;
  return tag;
}

int Operations::change_quality_tag() {
  int tag = change_quality_tag_;
  change_quality_tag_ = -1;
  return tag;
}