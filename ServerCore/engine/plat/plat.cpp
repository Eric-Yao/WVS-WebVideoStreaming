#include "plat.h"


    int   bGetTime(){return time(NULL);}

void bSleep(unsigned int time) {
  if ( 0==time ) {
    sched_yield();
  } else {
    sleep( time/1000 );
    usleep( time%1000*1000 );
  }
  return ;
}

 void GLULookAt(GLfloat eyex, GLfloat eyey, GLfloat eyez,
                   GLfloat centerx, GLfloat centery, GLfloat centerz,
                   GLfloat upx, GLfloat upy, GLfloat upz)
    {
        GLfloat m[16];
        GLfloat x[3], y[3], z[3];
        GLfloat mag;
        
        /* Make rotation matrix */
        
        /* Z vector */
        z[0] = eyex - centerx;
        z[1] = eyey - centery;
        z[2] = eyez - centerz;
        mag = sqrt(z[0] * z[0] + z[1] * z[1] + z[2] * z[2]);
        if (mag) {          /* mpichler, 19950515 */
            z[0] /= mag;
            z[1] /= mag;
            z[2] /= mag;
        }
        
        /* Y vector */
        y[0] = upx;
        y[1] = upy;
        y[2] = upz;
        
        /* X vector = Y cross Z */
        x[0] = y[1] * z[2] - y[2] * z[1];
        x[1] = -y[0] * z[2] + y[2] * z[0];
        x[2] = y[0] * z[1] - y[1] * z[0];
        
        /* Recompute Y = Z cross X */
        y[0] = z[1] * x[2] - z[2] * x[1];
        y[1] = -z[0] * x[2] + z[2] * x[0];
        y[2] = z[0] * x[1] - z[1] * x[0];
        
        /* mpichler, 19950515 */
        /* cross product gives area of parallelogram, which is < 1.0 for
         * non-perpendicular unit-length vectors; so normalize x, y here
         */
        
        mag = sqrt(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
        if (mag) {
            x[0] /= mag;
            x[1] /= mag;
            x[2] /= mag;
        }
        
        mag = sqrt(y[0] * y[0] + y[1] * y[1] + y[2] * y[2]);
        if (mag) {
            y[0] /= mag;
            y[1] /= mag;
            y[2] /= mag;
        }
        
#define M(row,col)  m[col*4+row]
        M(0, 0) = x[0];
        M(0, 1) = x[1];
        M(0, 2) = x[2];
        M(0, 3) = 0.0;
        M(1, 0) = y[0];
        M(1, 1) = y[1];
        M(1, 2) = y[2];
        M(1, 3) = 0.0;
        M(2, 0) = z[0];
        M(2, 1) = z[1];
        M(2, 2) = z[2];
        M(2, 3) = 0.0;
        M(3, 0) = 0.0;
        M(3, 1) = 0.0;
        M(3, 2) = 0.0;
        M(3, 3) = 1.0;
#undef M
        glMultMatrixf(m);
        
        /* Translate Eye to Origin */
        glTranslatef(-eyex, -eyey, -eyez);
        
    }
    
    void GLUPerspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar)
    {
        //glMatrixMode(GL_PROJECTION);
        //glLoadIdentity();
        
        GLfloat xmin, xmax, ymin, ymax;
        ymax = zNear * tan(fovy * 3.1415926 / 360.0);
        ymin = -ymax;
        xmin = ymin * aspect;
        xmax = ymax * aspect;
        
        //printf("fovy:%f, aspect:%f\n", fovy, aspect);
        //printf("(%f, %f, %f, %f, %f, %f )\n", xmin, xmax, ymin, ymax, zNear, zFar);

         glFrustum(xmin, xmax, ymin, ymax, zNear, zFar);
    }

bool bFCreate(const char* FileName) {
		int fp;
		/*d8 buffer[BUFFER_SIZE];*/
		//if((fp=open(FileName,O_CREAT | O_TRUNC | O_EXCL | O_RDWR|O_CREAT , S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH ))==-1) 
		if((fp = open(FileName, (O_CREAT | O_TRUNC | O_EXCL | O_RDWR), (S_IRWXU | S_IRWXG | S_IRWXO))) == -1)
		{
			fprintf(stderr,"dFL_Creat  %s Error:%s\n",FileName,strerror(errno));
			//close(fp);
			return false;
		} 
		close(fp); 
		return true;

	}


bool NonBlockSocketCheckConnect(int socket) {
 int len=4;
 int val=-1;
 int ret=-1;

 ret=getsockopt(socket, SOL_SOCKET, SO_ERROR, (char*)&val, (socklen_t*)&len);
 if (ret<0) {
   return false;
 } else {
   if (val == 0) {
     printf("Ret:%d, Val:%d`````````````````````````\n", ret, val);
     return true;
    } else {
      return false;
    }
  }
  return true;
}


Vector3DF& Vector3DF::normalize() {
  float l = x*x + y*y + z*z;
  if (l == 0)
    return *this;
  l = 1.0f /sqrtf(l);
  x *= l;
  y *= l;
  z *= l;
  return *this;
}

void Vector3DF::rotateXZBy(float degrees, const Vector3DF& center) {
  degrees *= kDegTorad32;
  float cs = (float)cos(degrees);
  float sn = (float)sin(degrees);
  x -= center.x;
  z -= center.z;
  set(x*cs - z*sn, y, x*sn + z*cs);
  x += center.x;
  z += center.z;
};

