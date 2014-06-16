#ifdef _WIN32
#include <windows.h>
#endif

#include <dake/gl/gl.hpp>
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


static gl::program *display_program;
static gl::vertex_array *mesh;


static void load_wavefront(const char *fname, std::vector<vec3> &p, std::vector<vec3> &n)
{
  std::ifstream file(fname);
  if (!file.is_open()) {
    throw std::runtime_error("Could not load mesh (could not open file)");
  }

  std::vector<vec3> vertices, normals;

  std::string str_line;
  while (std::getline(file, str_line, '\n')) {
    std::stringstream line(str_line);

    std::string deftype;
    line >> deftype;

    if (deftype == "v") {
      vec3 vertex;
      line >> vertex.x() >> vertex.y() >> vertex.z();
      vertices.push_back(vertex);
    } else if (deftype == "vn") {
      vec3 normal;
      line >> normal.x() >> normal.y() >> normal.z();
      normals.push_back(normal);
    } else if (deftype == "f") {
      vec3 *pos[3] = {nullptr}, *nrm[3] = {nullptr};

      std::string entry;
      for (int corner = 0; std::getline(line, entry, ' '); corner++) {
        if (entry.empty()) {
          corner--;
          continue;
        }

        if (corner >= 3) {
          throw std::runtime_error("Could not load mesh (not triangualized)");
        }

        std::stringstream entry_stream(entry);
        std::string indexstr;
        for (int i = 0; std::getline(entry_stream, indexstr, '/'); i++) {
          int index;
          char *end;

          if (indexstr.empty()) {
            index = -1;
          } else {
            index = strtol(indexstr.c_str(), &end, 10);
            if (*end) {
              throw std::runtime_error("Could not load mesh (invalid index given, is not a number)");
            }
          }

          if (index > 0) {
            switch (i) {
              case 0: pos[corner] = &vertices[index - 1]; break;
              case 1: break;
              case 2: nrm[corner] = &normals[index - 1]; break;
              default: throw std::runtime_error("Could not load mesh (invalid number of vertex attributes given)");
            }
          }
        }

        if (!pos[corner]) {
          throw std::runtime_error("Could not load mesh (no vertex position given)");
        }
      }

      bool backwards = false;
      if (nrm[0] || nrm[1] || nrm[2]) {
        // gotta love gcc
        vec3 *normal = nrm[0] ?: nrm[1] ?: nrm[2];

        vec3 nat_norm = (*pos[1] - *pos[0]).cross(*pos[2] - *pos[0]);
        backwards = (nat_norm.dot(*normal) < 0.f);
      }

      for (int i = backwards ? 2 : 0; backwards ? i >= 0 : i < 3; backwards ? i-- : i++) {
        p.push_back(*pos[i]);
        if (nrm[i]) {
          n.push_back(*nrm[i]);
        }
      }
    }
  }
}


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
    "uniform mat4 mvp;\n"

    "void main(void)\n"
    "{\n"
    "    gl_Position = mvp * vec4(in_position, 1.0);\n"
    "    vf_normal = in_normal;\n"
    "}"
  );

  fsh.source(
    "#version 150 core\n"
    "in vec3 vf_normal;\n"
    "out vec4 out_color;\n"

    "void main(void)\n"
    "{\n"
    "    out_color = vec4(normalize(vf_normal), 1.0);\n"
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

  display_program->bind_frag("out_color", 0);

  if (!display_program->link()) {
    delete display_program;
    display_program = nullptr;

    throw std::runtime_error("Could not link program");
  }


  std::vector<vec3> positions, normals;
  load_wavefront("mesh.obj", positions, normals);

  mesh = new gl::vertex_array;
  mesh->set_elements(positions.size());

  gl::vertex_attrib *va_p = mesh->attrib(0);
  va_p->format(3);
  va_p->data(positions.data());
  va_p->load();

  gl::vertex_attrib *va_n = mesh->attrib(1);
  va_n->format(3);
  va_n->data(normals.data());
  va_n->load();
}


void BoxDrawable::visualize(::view::GlRenderer &rnd, ::view::GlutWindow &)
{
  Box &box = dynamic_cast<Box &>(*obj);

  mat4 mvp = rnd.projection() * rnd.camera();
  mvp.translate(box.position());
  mvp.rotate(box.angle(), vec3(0.f, 1.f, 0.f));

  display_program->use();
  display_program->uniform<mat4>("mvp") = mvp;

  mesh->draw(GL_TRIANGLES);
}
