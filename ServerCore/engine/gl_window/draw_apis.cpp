

#include "draw_apis.h" //gl_window
#include "tcp_media.h"


#define WK_ESCAPE 27
#define WK_W      119
#define WK_S      115
#define WK_A      97
#define WK_D      100

#define WK_J  106  //left
#define WK_L  108  //right
#define WK_I  105  //up
#define WK_K  107  //down


#define WK_SPACE  32
#define WK_NUM_0  48


char picture_name[128];
int tag;
pthread_mutex_t glBuffMutex;
Draw * kExample;
char* gJpegContentBuffer = NULL;
int gJpegBufferLen=0;

int gJpeFileSize = 0;
void game_loop();
void game_resize_scene(int width, int height);
void key_pressed(unsigned char key, int x, int y);
void mouse_pressed(int button, int state, int x, int y);
void mouse_motion( int x, int y);


void drawnPngFile() {
  if(tag >= 200)
    tag = 0;
  memset(picture_name,0x0,128);
  sprintf(picture_name,"/Library/WebServer/Documents/test/images/pic_%d.jpg", tag ++ );

  jpeg_write_reset();

  pthread_mutex_lock(&glBuffMutex);
  //memset(gJpegContentBuffer, 0x0, gJpegBufferLen);
  gJpeFileSize = jpeg_write_buffer(NULL, 40, gJpegContentBuffer, gJpegBufferLen);
  pthread_mutex_unlock(&glBuffMutex);
  return;
}

int get_jpgcontent_size() {
  return gJpegBufferLen;
}

Draw * setup_gl_environment(int argc, char ** argv, const char* filepath, int width, int height, char* buffer, int len, pthread_mutex_t buffmutex) {

  gJpegContentBuffer = buffer;
  gJpegBufferLen = len;
  jpeg_write_init(width, height);
  
  glBuffMutex = buffmutex;


  /*setup an example*/
  Vector3DF eyePos, focusPos;
  eyePos.set(0,10,20);
  focusPos.set(0,1,0);

  Vector3DF pos, rotate;
  pos.set(0,0,0);
  rotate.set(0,0,0);

  std::string path = filepath;
  std::string filename;

  kExample = new Draw(width, height);
  kExample->Initialize(argc, argv, (void*)game_loop, (void*)game_resize_scene,
                       (void*)key_pressed, (void*)mouse_pressed, (void*) mouse_motion);

  kExample->SetCamera(eyePos, focusPos);

  filename = filepath + std::string("resources/ground.bmp");
  kExample->AddObject(filename.c_str(), false, pos, rotate, 20, 0.1, 20 );

  filename = path + std::string("resources/wall.bmp");
  kExample->AddObject(filename.c_str(), false, pos.set(-10, 5, 0), rotate, 0.01, 10, 20 );

  filename = path + std::string("resources/wall.bmp");
  kExample->AddObject(filename.c_str(), false, pos.set(0, 5, -10), rotate, 20, 10, 0.01 );

  filename = path + std::string("resources/crate.bmp");
  kExample->AddObject(filename.c_str(), true, pos.set(5, 2, 5), rotate.set(1,0,0), 3, 3, 3 );

  filename = path + std::string("resources/crate2.bmp");
  kExample->AddObject(filename.c_str(), true, pos.set(5, 3, -5), rotate, 2, 2, 2 );

  filename = path + std::string("resources/crate3.bmp");
  kExample->AddObject(filename.c_str(), true, pos.set(-5, 3, 5), rotate, 2, 2, 2 );

  filename = path + std::string("resources/crate4.bmp");
  kExample->AddObject(filename.c_str(), true, pos.set(-5, 2, -5), rotate, 2, 2, 2 );


  kExample->StartLoop();
  return kExample;
}

void destroy_gl_environment() {
  delete kExample;
  kExample = NULL;
}

void game_loop() {
  kExample->PrepareRender();
  kExample->DrawGLScene();
  drawnPngFile();
  kExample->FinishRender();
  bSleep(30);
}

void game_resize_scene(int width, int height) {
  kExample->ReSizeGLScene(width, height);
}


/* The function called whenever a key is pressed. */
void key_pressed(unsigned char key, int x, int y) {
    printf("%d \n", key);

  switch (key) {
    case WK_ESCAPE:
      glutDestroyWindow(kExample->window());
      exit(0);
      break;
    case WK_W:
      kExample->MoveHorizon(1, 0);
      break;
    case WK_S:
      kExample->MoveHorizon(-1, 0);
      break;
    case WK_A:
      kExample->MoveHorizon(0, -1);
      break;
    case WK_D:
      kExample->MoveHorizon(0, 1);
      break;
   case WK_I:
      kExample->RotateVertical(1.0);
      break;
    case WK_K:
      kExample->RotateVertical(-1.0);
      break;
    case WK_J:
      kExample->RotateHorizon(1.0);
      break;
    case WK_L:
      kExample->RotateHorizon(-1.0);
      break;
    case WK_SPACE:
      break;
    case WK_NUM_0:
      break;
    default:
      break;
    }
}

/* The function called whenever a key is pressed. */
void mouse_pressed(int button, int state, int x, int y)
{
/*
    switch(button)
    {
        case 0:
        {
            // printf("mouse left is pressed %d : (x:%d; y:%d)\n", state,x,y);
            if(0 == state)
                BoxHero_touchActiveFunc(ITBH_TOUCH_BEGAN, (float) x, (float) y);
            else if(1==state)
                BoxHero_touchActiveFunc(ITBH_TOUCH_END, (float) x, (float) y);
        }
            break;
        case 2:
        {
            break;
            // printf("mouse right is pressed %d : (x:%d; y:%d)\n",  state,x,y);
            if(0 == state)
                BoxHero_touchActiveFunc(ITBH_TOUCH_MOUSE_RIGHT_BEGAN, (float) x, (float) y);
            else if(1==state)
                BoxHero_touchActiveFunc(ITBH_TOUCH_MOUSE_RIGHT_END, (float) x, (float) y);
        }
            break;
        default:
            break;
    }
    */
}

void mouse_motion( int x, int y)
{
/*
   // printf(" mouseMotion (x:%d; y:%d)\n",x,y);
    if(x<0 || y <0 || x > g_Wind_Width || y > g_Wind_Height)
        return;
    
    BoxHero_touchActiveFunc(ITBH_TOUCH_MOVED, (float) x, (float) y);
    */
}
