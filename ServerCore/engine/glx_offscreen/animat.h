
#ifndef ANIMAT_H_
#define ANIMAT_H_

#include "include.h"
#include "plat.h"
#include "BmpImage.h"
#include "PrecisionTimer.h"
#include <iostream>
#include <vector>
#include <string>


#define kNbTile (10)

class Animat{
  struct ObjectInfo {
    bool static_tag;
    Vector3DF pos;
    Vector3DF rotate;
    Vector3DF box_vectors[24];
    GLuint texture;
  };
  typedef std::vector<ObjectInfo> Objects;

 public:
  Animat();
  ~Animat();
  void AddObject(const char* textureName, bool isStatic, Vector3DF pos, Vector3DF rotate,
                   float deltaX, float deltaY, float deltaZ );
  void SetupExample(const char* texture_path);
  void Update();
  void FinishRender();

  void SetCamera(const Vector3DF & eyePos, const Vector3DF & focusPos);
  void MoveHorizon(int forwardTag, int rightTag);
  void MoveVertical(float distance);
  void RotateHorizon(float degree);
  void RotateVertical(float distance);

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
  GLfloat   cube_tex_coords_[8];
  GLfloat   cube_vertices_[12];
  Vector3DF eye_pos_;
  Vector3DF focus_pos_;
  char fps_string_[128];

  float eye_delta_[2];
  int move_eye_counter_;
};
#endif
