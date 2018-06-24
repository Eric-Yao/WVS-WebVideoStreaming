
#include "apis.h"

int main(int argc, char *argv[])
{

  signal( SIGPIPE, SIG_IGN );
  signal(SIGUSR1,SIG_IGN);
  signal(SIGVTALRM,SIG_IGN);

  //start_init_texture_path("/Users/yongyao/YONG/OpenGL/CloudGaming/v4/");
  start_init_texture_path("/Users/yong/Documents/3D_Vision/CloudGaming/v4/");
  //start_with_windows_with_jpg(argc, argv, 600, 600);
  start_by_glx_with_jpg_with_noblock("111", 300, 300);
  
  
  //start_by_glx_with_mp4("111", 400, 400);
  //start_by_glx_with_jpg("111", 300, 300);
  return 1;
}

