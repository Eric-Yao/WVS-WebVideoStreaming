
#include "draw.h"


char fps_string[128];

Draw::Draw(int width, int height) : frame_width_(width), frame_height_(height) {
  rotate_counnter_ = 0;
  frame_number_ =0;
  cube_tex_coords_[0] = 1;
  cube_tex_coords_[1] = 1;
  cube_tex_coords_[2] = 0;
  cube_tex_coords_[3] = 1;
  cube_tex_coords_[4] = 1;
  cube_tex_coords_[5] = 0;
  cube_tex_coords_[6] = 0;
  cube_tex_coords_[7] = 0;
}

Draw::~Draw() {
  for (int i = 0; i < objects_.size(); i++)
    glDeleteTextures(1, &objects_[i].texture);
}


void Draw::AddObject(const char* textureName, bool isStatic, Vector3DF pos, Vector3DF rotate,
                     float deltaX, float deltaY, float deltaZ ) {
  Vector3DF min, max;
  min.set(-deltaX/2,-deltaY/2,-deltaZ/2);
  max.set(deltaX/2,deltaY/2,deltaZ/2);

  ObjectInfo temp;
  temp.static_tag = isStatic;
  temp.pos = pos;
  temp.rotate = rotate;
  CreateBox(temp.box_vectors, min, max);
  LoadTextures(textureName, &temp.texture);
  objects_.push_back(temp);
}


void Draw::Initialize(int argc, char ** argv, void* loopFunc, void* resizeFunc,
                      void* keyPressed, void* mousePressed, void* mouseMotion)
{
  glutInit(&argc, argv);
  timer_ = new PrecisionTimer();
  
  /* Select type of Display mode: Double buffer, RGBA color, Alpha components supported, Depth buffer */  
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);  

  /* get a 640 x 480 window */
  glutInitWindowSize(frame_width_, frame_height_);

  /* the window starts at the upper left corner of the screen */
  glutInitWindowPosition(0, 0);  

  /* Open a window */  
  window_ = glutCreateWindow("Ground");  

  /* Register the function to do all our OpenGL drawing. */
  glutDisplayFunc((CALLBACK_Loop)loopFunc);

  /* Even if there are no events, redraw our gl scene. */
  glutIdleFunc((CALLBACK_Loop)loopFunc);
  
  glutKeyboardFunc((CALLBACK_KeyPressed)keyPressed);
  glutMouseFunc((CALLBACK_MousePressed)mousePressed);
  glutMotionFunc((CALLBACK_MouseMotion)mouseMotion);

  /* Register the function called when our window is resized. */
  glutReshapeFunc((CALLBACK_Resize)resizeFunc);

  /* glutFullScreen(); */
  /* Initialize our window. */
  InitGL();
}

void Draw::StartLoop() {
  /* Start Event Processing Engine */  
  glutMainLoop();  
}



////////////////////////////////////////////////////

void Draw::InitGL()
{
  glClearColor(0.0f, 1.0f, 1.0f, 1.0f);		// This Will Clear The Background Color To Black
  glClearDepth(1.0);				// Enables Clearing Of The Depth Buffer
  glDepthFunc(GL_LESS);				// The Type Of Depth Test To Do
  glEnable(GL_TEXTURE_2D);			// Enable Texture Mapping  
  glEnable(GL_DEPTH_TEST);			// Enables Depth Testing
  glShadeModel(GL_SMOOTH);			// Enables Smooth Color Shading
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();				    // Reset The Projection Matrix
  glMatrixMode(GL_MODELVIEW);

  timer_ = new PrecisionTimer();
  timer_->start();
}

/*
 Edges are stored in this way:
    /6--------/7    	 | Y+                        
  /  |      / |     	 |             
 /   |     /  |     	 |             
 2---------3  |     	 |--------- X+     
 |   4- - -| -5         /       
 |  /      |  /        /     
 |/        | /        / Z+			     
 0---------1/          		
 
 2: MinVertex
 5: MaxVertex
 
 Front: 1, 0, 3, 2
 Back:  4, 5, 6, 7
 Left:  0, 4, 2, 6
 Right: 5, 1, 7, 3	
 Top:   3, 2, 7, 6
 Bottom:5, 4, 1, 0
 */
void Draw::CreateBox(Vector3DF * edges, Vector3DF min, Vector3DF max) {
  Vector3DF vertex[8];

  vertex[0].set(min.x,min.y, max.z);
  vertex[1].set(max.x,min.y,max.z);
  vertex[2].set(min.x,max.y,max.z);
  vertex[3].set(max);
  vertex[4].set(min);
  vertex[5].set(max.x,min.y,min.z);
  vertex[6].set(min.x,max.y,min.z);
  vertex[7].set(max.x,max.y,min.z);

  edges[0].set(vertex[4]);
  edges[1].set(vertex[5]);
  edges[2].set(vertex[6]);
  edges[3].set(vertex[7]);
    
  /*Left: 0, 4, 2, 6*/
  edges[4].set(vertex[0]);
  edges[5].set(vertex[4]);
  edges[6].set(vertex[2]);
  edges[7].set(vertex[6]);
    
  /* Right:  5, 1, 7, 3	*/
  edges[8].set(vertex[5]);
  edges[9].set(vertex[1]);
  edges[10].set(vertex[7]);
  edges[11].set(vertex[3]);
    
  /* Bottom:  5, 4, 1, 0	*/
  edges[12].set(vertex[5]);
  edges[13].set(vertex[4]);
  edges[14].set(vertex[1]);
  edges[15].set(vertex[0]);
    
    
  /* Top:   3, 2, 7, 6	*/
  edges[16].set(vertex[3]);
  edges[17].set(vertex[2]);
  edges[18].set(vertex[7]);
  edges[19].set(vertex[6]);
    
  /* Front: 3, 2, 1, 0*/
  edges[20].set(vertex[3]);
  edges[21].set(vertex[2]);
  edges[22].set(vertex[1]);
  edges[23].set(vertex[0]);

}

void Draw::LoadTextures(const char* fileName, GLuint* texture) {
  BmpImage* loader = new BmpImage(fileName);
  glGenTextures(1, texture);
  glBindTexture(GL_TEXTURE_2D, *texture);   // 2d texture (x and y size)
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // scale linearly when image smalled than texture
  glTexImage2D(GL_TEXTURE_2D, 0, 3, loader->getWidth(), loader->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, loader->getBitmap() );
  delete loader;
}

void Draw::DrawObject(int index) {
  glPushMatrix();
  glTranslatef(objects_[index].pos.x, objects_[index].pos.y, objects_[index].pos.z);
  if (objects_[index].static_tag == true)
    glRotatef(rotate_counnter_, objects_[index].rotate.x, objects_[index].rotate.y, objects_[index].rotate.z);

  glBindTexture(GL_TEXTURE_2D, objects_[index].texture);
  glBegin(GL_TRIANGLE_STRIP);				
  glColor3f(1.0f, 1.0f, 1.0f);


/*
  for (int i =0; i< 6; i++) {
    for(int j=0;j<4;j++) {
         cube_vertices_[j*3] = objects_[index].box_vectors[4*i + j].x;
         cube_vertices_[j*3 + 1] = objects_[index].box_vectors[4*i + j].y;
         cube_vertices_[j*3 + 1] = objects_[index].box_vectors[4*i + j].z;
     }
     glTexCoordPointer(2, GL_FLOAT, 0, cube_tex_coords_);
     glVertexPointer(3, GL_FLOAT, 0, cube_vertices_);
     glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
     glBindTexture(GL_TEXTURE_2D, 0);
  }
*/
   for (int i =0; i< 6; i++) {
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(objects_[index].box_vectors[i*4].x, objects_[index].box_vectors[i*4].y, objects_[index].box_vectors[i*4].z );
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(objects_[index].box_vectors[i*4+1].x, objects_[index].box_vectors[i*4+1].y, objects_[index].box_vectors[i*4+1].z );
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(objects_[index].box_vectors[i*4+2].x, objects_[index].box_vectors[i*4+2].y, objects_[index].box_vectors[i*4+2].z );
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(objects_[index].box_vectors[i*4+3].x, objects_[index].box_vectors[i*4+3].y, objects_[index].box_vectors[i*4+3].z );

   }
  glEnd();
  glPopMatrix();
}

void Draw::ReSizeGLScene(int width, int height) {
  frame_width_ = width;
  frame_height_ = height;

  if (frame_height_==0)				// Prevent A Divide By Zero If The Window Is Too Small
    frame_height_=1;

  glViewport(0, 0, frame_width_, frame_height_);	// Reset The Current Viewport And Perspective Transformation

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  GLUPerspective(45.0f,(GLfloat)frame_width_/(GLfloat)frame_height_,0.1f,100.0f);	// Calculate The Aspect Ratio Of The Window
  glMatrixMode(GL_MODELVIEW);
}

void Draw::PrepareRender() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
  glLoadIdentity();				// Reset The View
  glDisable (GL_BLEND);
  glEnable(GL_DEPTH_TEST);
  GLULookAt(eye_pos_.x, eye_pos_.y, eye_pos_.z, focus_pos_.x, focus_pos_.y, focus_pos_.z, 0, 1, 0);
}

void Draw::FinishRender() {
  frame_number_++;
  rotate_counnter_++;

  glutSwapBuffers();
  if(frame_number_ > 60)
  {
      timer_->stop();
      float delata = timer_->getDuration();
      sprintf(fps_string,"Fps: %3.1f", (frame_number_ /delata ) );
      glutSetWindowTitle(fps_string);
      frame_number_ = 0;
      timer_->start();
  }
}

void Draw::DrawGLScene() {
  for (int i = 0; i < objects_.size(); i++)
    DrawObject(i);
}

void Draw::SetCamera(const Vector3DF & eyePos, const Vector3DF & focusPos) {
  eye_pos_ = eyePos;
  focus_pos_ = focusPos;
}


void Draw::MoveHorizon(int forwardTag, int rightTag) {
  Vector3DF forward = focus_pos_ - eye_pos_;

  forward.normalize();
  forward.y = 0;
  forward.normalize();

  Vector3DF right = forward;

  if (forwardTag != 0) {
    if (forwardTag == -1)
      forward *= -1.0f;
    focus_pos_ += forward;
    eye_pos_ += forward;
  }

  if (rightTag != 0) {
    Vector3DF mid;
    mid.set(0.0f, 0.0f, 0.0f);
    right.rotateXZBy(90.0f, mid);

    if (rightTag == -1) 
      right *= -1.0f;

    focus_pos_ += right;
    eye_pos_ += right;

    //printf("%f, %f ,%f \n", right.x, right.y, right.z);
  }
}


void Draw::MoveVertical(float distance) {
  focus_pos_.y += distance;
  eye_pos_.y += distance;
}

void Draw::RotateHorizon(float degree) {
  focus_pos_.rotateXZBy(degree, eye_pos_);
}

void Draw::RotateVertical(float distance) {
  focus_pos_.y += distance;
}


