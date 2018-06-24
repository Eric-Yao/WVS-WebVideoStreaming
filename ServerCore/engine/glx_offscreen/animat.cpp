
#include "animat.h"




Animat::Animat() {
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
  timer_ = new PrecisionTimer();
  timer_->start();
  eye_delta_[0] = 0.02;
  eye_delta_[1] = 0.02;
  move_eye_counter_ = 0;
}

Animat::~Animat() {
  for (int i = 0; i < objects_.size(); i++)
    glDeleteTextures(1, &objects_[i].texture);
  delete timer_;
}

void Animat::SetupExample(const char* texture_path) {
Vector3DF eyePos, focusPos, pos, rotate;
  eyePos.set(0,10,20);
  focusPos.set(0,1,0);
  pos.set(0,0,0);
  rotate.set(0,0,0);

  std::string path = texture_path;
  std::string filename;
  SetCamera(eyePos,focusPos);
  
  filename = path + std::string("resources/ground.bmp");
  AddObject(filename.c_str(), false, pos, rotate, 20, 0.1, 20 );

  filename = path + std::string("resources/wall.bmp");
  AddObject(filename.c_str(), false, pos.set(-10, 5, 0), rotate, 0.01, 10, 20 );

  filename = path + std::string("resources/wall.bmp");
  AddObject(filename.c_str(), false, pos.set(0, 5, -10), rotate, 20, 10, 0.01 );

  filename = path + std::string("resources/crate.bmp");
  AddObject(filename.c_str(), true, pos.set(5, 2, 5), rotate.set(1,0,0), 3, 3, 3 );

  filename = path + std::string("resources/crate2.bmp");
  AddObject(filename.c_str(), true, pos.set(5, 3, -5), rotate, 2, 2, 2 );

  filename = path + std::string("resources/crate3.bmp");
  AddObject(filename.c_str(), true, pos.set(-5, 3, 5), rotate, 2, 2, 2 );

  filename = path + std::string("resources/crate4.bmp");
  AddObject(filename.c_str(), true, pos.set(-5, 2, -5), rotate, 2, 2, 2 );
}


void Animat::AddObject(const char* textureName, bool isStatic, Vector3DF pos, Vector3DF rotate,
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

////////////////////////////////////////////////////

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
void Animat::CreateBox(Vector3DF * edges, Vector3DF min, Vector3DF max) {
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

void Animat::LoadTextures(const char* fileName, GLuint* texture) {
  BmpImage* loader = new BmpImage(fileName);
  glGenTextures(1, texture);
  glBindTexture(GL_TEXTURE_2D, *texture);   // 2d texture (x and y size)
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // scale linearly when image smalled than texture
  glTexImage2D(GL_TEXTURE_2D, 0, 3, loader->getWidth(), loader->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, loader->getBitmap() );
  delete loader;
}

void Animat::DrawObject(int index) {
  glPushMatrix();
  glTranslatef(objects_[index].pos.x, objects_[index].pos.y, objects_[index].pos.z);
  if (objects_[index].static_tag == true)
    glRotatef(rotate_counnter_, objects_[index].rotate.x, objects_[index].rotate.y, objects_[index].rotate.z);

  glBindTexture(GL_TEXTURE_2D, objects_[index].texture);
  glBegin(GL_TRIANGLE_STRIP);				
  glColor3f(1.0f, 1.0f, 1.0f);
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

void Animat::Update() {

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
  glLoadIdentity();				// Reset The View
  glDisable (GL_BLEND);
  glEnable(GL_DEPTH_TEST);

  if (move_eye_counter_ == 0) {
    eye_delta_[0] = -4.0 * eye_delta_[1];
  } else {
    eye_delta_[0] += eye_delta_[1];
  }

  GLULookAt(eye_pos_.x , eye_pos_.y, eye_pos_.z, focus_pos_.x, focus_pos_.y, focus_pos_.z, 0, 1, 0);
  for (int i = 0; i < objects_.size(); i++)
     DrawObject(i);
  
  //frame_number_++;
  rotate_counnter_++;

  move_eye_counter_ ++;
  if (move_eye_counter_ == 9)
    move_eye_counter_ = 0;


}

void Animat::FinishRender() {
  frame_number_++;
  rotate_counnter_++;

  glutSwapBuffers();
  if(frame_number_ > 60)
  {
      timer_->stop();
      float delata = timer_->getDuration();
      sprintf(fps_string_,"Fps: %3.1f", (frame_number_ /delata ) );
      glutSetWindowTitle(fps_string_);
      frame_number_ = 0;
      timer_->start();
  }
}

void Animat::SetCamera(const Vector3DF & eyePos, const Vector3DF & focusPos) {
  eye_pos_ = eyePos;
  focus_pos_ = focusPos;
}


void Animat::MoveHorizon(int forwardTag, int rightTag) {
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


void Animat::MoveVertical(float distance) {
  focus_pos_.y += distance;
  eye_pos_.y += distance;
}

void Animat::RotateHorizon(float degree) {
  focus_pos_.rotateXZBy(degree, eye_pos_);
}

void Animat::RotateVertical(float distance) {
  focus_pos_.y += distance;
}


