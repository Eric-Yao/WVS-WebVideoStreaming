
#include "apis.h"


#include "animat.h"
#include "record.h"
#include "pbuffer.h"
#include "tcp_media.h"
#include "tcp_streaming.h"
#include "tcp_operations.h"

#include "draw.h" //gl_window mode
#include "draw_apis.h" //gl_window mode

#include "statistics.h"

pthread_mutex_t gBuffMutex;
char *gContentBuffer = NULL;
static int kBufferLen = 1024*60;
std::string kTexturPath;

void setup_buffer_and_mutex() {
  pthread_mutex_init( &gBuffMutex, NULL);
  gContentBuffer = new char[kBufferLen * 2];
}

void remove_buffer_and_mutex() {
  delete[] gContentBuffer;
  pthread_mutex_destroy( &gBuffMutex);
}


static void *server(void *arg) {
   bSleep(100);
   Record * ptr = static_cast<Record*>(arg);
   Media* server = new Media(gContentBuffer, ptr, gBuffMutex);
   server->setup_block_server(9960);
   return NULL;
}

void start_init_texture_path(char * path) {
  kTexturPath = path;
}

void start_with_windows_with_jpg(int argc, char **argv, int width, int height) {
  pthread_t pid;

  setup_buffer_and_mutex();

  //pthread_create( &pid, NULL, server ,NULL);
  //bSleep(100);
  setup_gl_environment(argc, argv, kTexturPath.c_str(), width, height, gContentBuffer, kBufferLen, gBuffMutex);
  destroy_gl_environment();

  remove_buffer_and_mutex();

  return;
}

void start_by_glx_with_jpg(char* filepath, int width, int height) {
  pthread_t pid;

  setup_buffer_and_mutex();

  Record* jpg = new Record(width, height, filepath, gContentBuffer, kBufferLen, gBuffMutex);
  jpg->InitJpeg();

  pthread_create( &pid, NULL, server,static_cast<void*>(jpg));
  bSleep(4000);

  /*first setup pbuff*/
  PBuffer* pbuf = new PBuffer(width, height);
  pbuf->Setup();

  /*then setup game*/
  Animat * game = new Animat();
  game->SetupExample(kTexturPath.c_str());


  unsigned int buffer_len = 1024*5;
  char sendbuff[1024*5];
  char recvbuff[1024*5];
  Operations * operation_server = new Operations(kTexturPath.c_str());
  operation_server->Setup(9940);

  while (1) {
    pbuf->Start();
    game->Update();
    pbuf->Finish();
    jpg->WriteJpeg();

    operation_server->Run(recvbuff, sendbuff, buffer_len);
    bSleep(20);
  }
  printf("done\n");

  pbuf->Release();
  delete jpg;
  delete pbuf;
  delete game;
  remove_buffer_and_mutex();
}

void start_by_glx_with_mp4(char* filepath, int width, int height) {

  setup_buffer_and_mutex();

  Record* video = new Record(width, height, filepath, gContentBuffer, kBufferLen, gBuffMutex);
  video->InitMp4();

  //pthread_create( &pid, NULL, server,static_cast<void*>(video));
  //bSleep(100);

  /*first setup pbuff*/
  PBuffer* pbuf = new PBuffer(width, height);
  pbuf->Setup();

  /*then setup game*/
  Animat * game = new Animat();
  game->SetupExample(kTexturPath.c_str());


  while (1) {
    pbuf->Start();
    game->Update();
    pbuf->Finish();
    video->WriteMp4File();
    bSleep(100);
  }
  printf("done\n");

  pbuf->Release();
  delete video;
  delete pbuf;
  delete game;
  remove_buffer_and_mutex();
}

void start_by_glx_with_jpg_with_noblock(char* filepath, int width, int height) {

  setup_buffer_and_mutex();

  Record* jpg = new Record(width, height, filepath, gContentBuffer, kBufferLen, gBuffMutex);
  jpg->InitJpeg();

  /*first setup pbuff*/
  PBuffer* pbuf = new PBuffer(width, height);
  pbuf->Setup();

  /*then setup game*/
  Animat * game = new Animat();
  game->SetupExample(kTexturPath.c_str());


  /*setup non-block tcp-server*/
  unsigned int stream_buffer_len = 1024*60;
  char stream_sendbuff[1024*60];
  char stream_recvbuff[1024*60];
  Streaming * noblock_server = new Streaming();
  noblock_server->Setup(9920);

  unsigned int operation_buffer_len = 1024*10;
  char operation_sendbuff[1024*10];
  char operation_recvbuff[1024*10];
  Operations * operation_server = new Operations(kTexturPath.c_str());
  operation_server->Setup(9930);
  operation_server->RegisterEnvironment(game, jpg);


  int statistics_buffer_len = 1024*5;
  char glframeinfo_sendbuff[1024*5];
  char useractivity_sendbuff[1024*5];
  Statistics* stats = new Statistics(kTexturPath.c_str());


  while (1) {
    pbuf->Start();
    game->Update();
    pbuf->Finish();
    jpg->WriteJpeg();

    //memset(stream_sendbuff, 0x0, stream_buffer_len);
    //memset(stream_recvbuff, 0x0, stream_buffer_len);

    //memset(operation_sendbuff, 0x0, operation_buffer_len);
    //memset(operation_recvbuff, 0x0, operation_buffer_len);

    stats->UpdateGlFrameInfo(jpg->jpg_quality(), noblock_server->sent_size());
    stats->UpdateUserInfo(operation_server->move_tag(), operation_server->rotate_tag(), operation_server->change_quality_tag());
    stats->StoreInfo(glframeinfo_sendbuff, useractivity_sendbuff, statistics_buffer_len);

    noblock_server->SetContentBuff(gContentBuffer, jpg->jpg_file_size());
    noblock_server->Run(stream_recvbuff, stream_sendbuff, stream_buffer_len);

    operation_server->SetStatistics(glframeinfo_sendbuff, useractivity_sendbuff,
                                    stats->gl_frame_info_log_len(), stats->user_activity_log_len());
    operation_server->Run(operation_recvbuff, operation_sendbuff, operation_buffer_len);

    bSleep(1);
  }
  printf("done\n");

  pbuf->Release();
  delete jpg;
  delete pbuf;
  delete game;
  remove_buffer_and_mutex();
}


