
#ifndef _PLAT_
#define _PLAT_

#include "include.h"

const float kPI32		= (3.14159265359f);
const float kHallPi32	= (kPI32/2.0f);
const float kDegTorad32 = (kPI32/180.0f);

enum MoveTag {
  kMoveForward = 0,
  kMoveBack,
  kMoveLeft,
  kMoveRight
};
enum RotateTag {
  kRotateUp = 0,
  kRotateDown,
  kRotateLeft,
  kRotateRight
};
enum ChangeQualityTag {
  kQualityReset = 0,
  kQualityIncrease,
  kQualityDecrease
};

void GLULookAt(GLfloat eyex, GLfloat eyey, GLfloat eyez,
                    GLfloat centerx, GLfloat centery, GLfloat centerz,
                    GLfloat upx, GLfloat upy, GLfloat upz);
void GLUPerspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar);

int  bGetTime();

void bSleep(unsigned int time);
bool bFCreate(const char* FileName);

bool NonBlockSocketCheckConnect(int socket);


struct Vector3DF {
  float x;
  float y;
  float z;
 // Vector3DF();
 // Vector3DF(const float nx, const float ny, const float nz) {set(nx, ny, nz);}
 // ~Vector3DF(){}

  Vector3DF& operator=(const Vector3DF& other){set(other.x, other.y, other.z); return *this;}
  Vector3DF& operator+=(const Vector3DF& other) { x+=other.x; y+=other.y; z+=other.z; return *this; }
  Vector3DF operator+(const Vector3DF& other) const { Vector3DF temp; return temp.set(x + other.x, y + other.y, z + other.z); }
  Vector3DF& operator-=(const Vector3DF& other) { x-=other.x; y-=other.y; z-=other.z; return *this; }
  Vector3DF operator-(const Vector3DF& other) const { Vector3DF temp; return temp.set(x - other.x, y - other.y, z - other.z); }
  Vector3DF& operator*=(const float v) { x*=v; y*=v; z*=v; return *this; }
  Vector3DF operator*(const float v) const { Vector3DF temp; return temp.set(x*v, y*v, z*v); }

  Vector3DF& set(const float nx, const float ny, const float nz) {x=nx; y=ny; z=nz; return *this;}
  Vector3DF& set(const Vector3DF & p) { x=p.x; y=p.y; z=p.z; return *this;}

  Vector3DF& normalize();
  void rotateXZBy(float degrees, const Vector3DF& center);
};

#endif