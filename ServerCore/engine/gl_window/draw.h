
#ifndef _Draw_
#define _Draw_

#include "include.h"
#include "plat.h"
#include "BmpImage.h"
#include "PrecisionTimer.h"
#include <iostream>
#include <vector>


#define kNbTile (10)

typedef void (*CALLBACK_Loop)();
typedef void (*CALLBACK_Resize)(int w, int h);
typedef void (*CALLBACK_KeyPressed)(unsigned char key, int x, int y);
typedef void (*CALLBACK_MousePressed)(int button, int state, int x, int y) ;
typedef void (*CALLBACK_MouseMotion)( int x, int y);


class Draw
{
  struct ObjectInfo {
    bool static_tag;
    Vector3DF pos;
    Vector3DF rotate;
    Vector3DF box_vectors[24];
    GLuint texture;
  };

  typedef std::vector<ObjectInfo> Objects;

  public:
    Draw(int width, int height);
    ~Draw();
    void AddObject(const char* textureName, bool isStatic, Vector3DF pos, Vector3DF rotate,
                   float deltaX, float deltaY, float deltaZ );
    void Initialize(int argc, char ** argv, void* loopFunc, void* resizeFunc,
                    void* keyPressed, void* mousePressed, void* mouseMotion);
    void StartLoop();

    void ReSizeGLScene(int width, int height);
    void PrepareRender();
    void FinishRender();
    void DrawGLScene();
    void SetCamera(const Vector3DF & eyePos, const Vector3DF & focusPos);

    void MoveHorizon(int forwardTag, int rightTag);
    void MoveVertical(float distance);
    void RotateHorizon(float degree);
    void RotateVertical(float distance);

    int frame_width() {return frame_width_;}
    int frame_height() {return frame_height_;}
    int window() {return window_;}

  private:
    void InitGL();
    void CreateBox(Vector3DF * edge, Vector3DF min, Vector3DF max);
    void LoadTextures(const char* fileName, GLuint* texture);
    void DrawObject(int index);
    
  private:
    Objects objects_;
    PrecisionTimer *timer_;
    float rotate_counnter_;
    float frame_number_;
    int frame_width_;
    int frame_height_;
    int window_;
    GLfloat   cube_tex_coords_[8];
    GLfloat   cube_vertices_[12];
    Vector3DF eye_pos_;
    Vector3DF focus_pos_;
};
#endif
