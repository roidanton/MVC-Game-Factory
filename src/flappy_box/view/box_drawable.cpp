#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glut.h>

#include "flappy_box/model/box.hpp"
#include "flappy_box/view/box_drawable.hpp"


using namespace flappy_box::model;
using namespace flappy_box::view;


void BoxDrawable::visualize(::view::GlRenderer &, ::view::GlutWindow &)
{
  Box &box = dynamic_cast<Box &>(*obj);

  // plz use ogl 3.3 core next time kthanx
  glPushMatrix();

  glTranslatef(box.position().x(), box.position().y(), box.position().z());
  glRotatef(box.angle(), 0.f, 0.f, 1.f);

  // based "using Z as the vertical axis"
  glRotatef(90.f, 1.f, 0.f, 0.f);

  glColor3f(.75f, .75f, .75f);

  // function pointers are best pointers
  (box.type() ? glutWireTeapot : glutWireCube)(2.f);

  glPopMatrix();
}
