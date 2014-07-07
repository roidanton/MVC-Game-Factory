#ifdef _WIN32
#include <windows.h>
#endif

#include <dake/gl/gl.hpp>
#include <dake/gl/obj.hpp>
#include <dake/gl/shader.hpp>
#include <dake/gl/vertex_array.hpp>
#include <dake/gl/vertex_attrib.hpp>
#include <dake/math/matrix.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <GL/glut.h>

#include "flappy_box/model/box.hpp"
#include "flappy_box/view/box_drawable.hpp"


using namespace flappy_box::model;
using namespace flappy_box::view;

using namespace dake;
using namespace dake::math;


// These are common for all objects of this class, therefore keep them static
// for all
static gl::program *display_program;
static gl::vertex_array *mesh;


BoxDrawable::BoxDrawable(void)
{
  if (display_program) {
    return;
  }


  gl::shader vsh(gl::shader::VERTEX), fsh(gl::shader::FRAGMENT);

  vsh.source(
    "#version 150 core\n"
    "in vec3 in_position;\n"
    "in vec3 in_normal;\n"
    "out vec3 vf_normal;\n"
    "out vec3 vf_coord;\n"
    "uniform mat4 mvp;\n"
    "uniform mat3 norm_mat;\n"

    "void main(void)\n"
    "{\n"
    "    gl_Position = mvp * vec4(in_position, 1.0);\n"
    "    vf_coord = in_position;\n"
    "    vf_normal = normalize(norm_mat * in_normal);\n"
    "}"
  );

  fsh.source(
    "#version 150 core\n"
    "in vec3 vf_normal;\n"
    "in vec3 vf_coord;\n"
    "out vec4 out_mi, out_hi;\n"
    "const vec3 light_dir = vec3(1.0, -3.0, 2.0);\n"
    "uniform float enlightenment;\n"

    "void main(void)\n"
    "{\n"
    "    vec2 z = vec2(0.0);\n"
    "    vec2 rc = vf_coord.xz / 500.0 + vec2(0.451, 0.4);\n"
    "    int i;\n"

    "    for (i = 0; i < 42; i++) {\n"
    "        vec2 v = vec2(dot(z, vec2(z.x, -z.y)), 2 * z.x * z.y) + rc;\n"
    "        if (length(v) > 2.) {\n"
    "            break;\n"
    "        }\n"
    "        z = v;\n"
    "    }\n"

    "    vec3 diff_color = (1.0 - float(i) / 42.0) * (vf_coord / 3.0 + vec3(1.0, 1.0, 1.0));\n"

    "    float diff_co = dot(normalize(vf_normal), normalize(-light_dir));\n"
    "    vec3 col = enlightenment * diff_co * diff_color;\n"

    "    out_mi = vec4(col, 1.0);\n"
    "    out_hi = vec4(col / 10.0, 1.0);\n"
    "}"
  );

  if (!vsh.compile() || !fsh.compile()) {
    throw std::runtime_error("Could not compile all shaders");
  }

  display_program = new gl::program;

  *display_program << vsh;
  *display_program << fsh;

  display_program->bind_attrib("in_position", 0);
  display_program->bind_attrib("in_normal", 1);

  display_program->bind_frag("out_mi", 0);
  display_program->bind_frag("out_hi", 1);

  if (!display_program->link()) {
    delete display_program;
    display_program = nullptr;

    throw std::runtime_error("Could not link program");
  }


  gl::obj m = gl::load_obj("assets/mesh.obj");
  if (m.sections.size() != 1) {
    throw std::runtime_error("Invalid mesh given: Does not have exactly one section");
  }

  gl::obj_section ms = m.sections.front();

  // Normalize the model to fit in a cube with a side length of 3 (because
  // that's the size of the model I wrote this code with)
  float scale = HUGE_VALF;
  for (int i = 0; i < 3; i++) {
    float s = 3.f / (m.upper_right[i] - m.lower_left[i]);
    if (s < scale) {
      scale = s;
    }
  }

  // And the cube should encompass the center
  vec3 translation = -(m.upper_right + m.lower_left) / 2;

  for (vec3 &pos: ms.positions) {
    pos = scale * (pos + translation);
  }

  mesh = ms.make_vertex_array(0, -1, 1);
}


void BoxDrawable::visualize(::view::GlRenderer &rnd, ::view::GlutWindow &)
{
  Box &box = dynamic_cast<Box &>(*obj);

  mat4 mvp = rnd.projection() * rnd.camera();
  mvp.translate(box.position());
  mvp.rotate(box.angle(), vec3(0.f, 1.f, 0.f));

  display_program->use();
  display_program->uniform<mat4>("mvp") = mvp;
  display_program->uniform<mat3>("norm_mat") = mat3(mvp).transposed_inverse();
  display_program->uniform<float>("enlightenment") = 5.f / (.2f + fabs(box.position().y()));

  mesh->draw(GL_TRIANGLES);
}
