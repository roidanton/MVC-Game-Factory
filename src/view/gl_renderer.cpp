#include <dake/gl/gl.hpp>
#include <dake/gl/framebuffer.hpp>
#include <dake/gl/shader.hpp>
#include <dake/gl/texture.hpp>
#include <dake/gl/vertex_array.hpp>
#include <dake/gl/vertex_attrib.hpp>
#include <dake/math/matrix.hpp>

#include <chrono>
#include <cmath>
#include <random>
#include <stdexcept>

# include "view/gl_renderer.hpp"
# include "view/glut_window.hpp"

# include "GL/freeglut.h"

using namespace ::view;

using namespace dake;
using namespace dake::math;


#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif


static gl::program *fb_prg;
static gl::vertex_array *fb_vertices;

const int STAR_COUNT = 4096;

static gl::program *star_prg;
static gl::vertex_array *stars;


GlRenderer::GlRenderer( std::shared_ptr< model::Game const > const& g )
: _game_model( g )
{}

void GlRenderer::init_with_context(void)
{
  glClearColor(0.f, 0.f, 0.f, 1.f);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);


  fb = new gl::framebuffer(2);

  // TMU 0 will be used by the blur FBO textures later; they "replace" the
  // second texture of this FBO
  (*fb)[0].set_tmu(1);
  (*fb)[1].set_tmu(0);


  blur_fbs[0] = new gl::framebuffer(1);
  blur_fbs[1] = new gl::framebuffer(1);


  gl::shader vsh(gl::shader::VERTEX), fsh(gl::shader::FRAGMENT);

  vsh.source(
    "#version 150 core\n"
    "in vec2 in_pos;\n"
    "out vec2 vf_pos;\n"

    "void main(void)\n"
    "{\n"
    "    gl_Position = vec4(in_pos, 0.0, 1.0);\n"
    "    vf_pos = (in_pos + vec2(1.0, 1.0)) / 2.0;\n"
    "}"
  );

  fsh.source(
    "#version 150 core\n"
    "in vec2 vf_pos;\n"
    "out vec4 out_color;\n"
    "uniform sampler2D fb_mi, fb_hi;\n"

    "void main(void)\n"
    "{\n"
    "    vec4 bloom = clamp(texture(fb_hi, vf_pos) * 10.0 - vec4(0.1, 0.1, 0.1, 0.0), 0.0, 1.0);\n"
    "    out_color = texture(fb_mi, vf_pos) + bloom;\n"
    "}"
  );

  if (!vsh.compile() || !fsh.compile()) {
    throw std::runtime_error("Could not compile all FB display shaders");
  }

  fb_prg = new gl::program;

  *fb_prg << vsh;
  *fb_prg << fsh;

  fb_prg->bind_attrib("in_pos", 0);
  fb_prg->bind_frag("out_color", 0);

  if (!fb_prg->link()) {
    throw std::runtime_error("Could not link FB display program");
  }


  gl::shader blur_x(gl::shader::FRAGMENT);
  blur_x.source(
    "#version 150 core\n"
    "in vec2 vf_pos;\n"
    "out vec4 out_color;\n"
    "uniform sampler2D input_tex;\n"
    "uniform float epsilon;\n"

    "vec4 access(float diff)\n"
    "{\n"
    "    return texture(input_tex, vec2(clamp(vf_pos.x + epsilon * diff, 0.0, 0.9999), vf_pos.y));\n"
    "}\n"

    "void main(void)\n"
    "{\n"
    "    out_color = access( 0.0) * 0.4\n"
    "              + access( 1.0) * 0.2\n"
    "              + access(-1.0) * 0.2\n"
    "              + access( 2.0) * 0.1\n"
    "              + access(-2.0) * 0.1;\n"
    "}"
  );

  gl::shader blur_y(gl::shader::FRAGMENT);
  blur_y.source(
    "#version 150 core\n"
    "in vec2 vf_pos;\n"
    "out vec4 out_color;\n"
    "uniform sampler2D input_tex;\n"
    "uniform float epsilon;\n"

    "vec4 access(float diff)\n"
    "{\n"
    "    return texture(input_tex, vec2(vf_pos.x, clamp(vf_pos.y + epsilon * diff, 0.0, 0.9999)));\n"
    "}\n"

    "void main(void)\n"
    "{\n"
    "    out_color = access( 0.0) * 0.4\n"
    "              + access( 1.0) * 0.2\n"
    "              + access(-1.0) * 0.2\n"
    "              + access( 2.0) * 0.1\n"
    "              + access(-2.0) * 0.1;\n"
    "}"
  );

  if (!blur_x.compile() || !blur_y.compile()) {
    throw std::runtime_error("Could not compile blur shaders");
  }

  for (int i: {0, 1}) {
    blur_prg[i] = new gl::program;

    *blur_prg[i] << vsh;
    *blur_prg[i] << (i ? blur_y : blur_x);

    blur_prg[i]->bind_attrib("in_pos", 0);
    blur_prg[i]->bind_frag("out_color", 0);

    if (!blur_prg[i]->link()) {
      throw std::runtime_error("Could not link blur program");
    }
  }


  gl::shader star_vsh(gl::shader::VERTEX), star_fsh(gl::shader::FRAGMENT);

  star_vsh.source(
    "#version 150 core\n"
    "in vec3 in_pos, in_color;\n"
    "out float vf_zpos;\n"
    "out vec3 vf_color;\n"
    "uniform float zdiff;\n"
    "uniform mat4 mvp;\n"

    "void main(void)\n"
    "{\n"
    "    float zpos = mod(in_pos.z + zdiff, 10.0);\n"
    "    vf_zpos = zpos / 10.0;\n"
    "    gl_Position = mvp * vec4(in_pos.xy, zpos - 5.0, 1.0);\n"
    "    vf_color = in_color;\n"
    "}"
  );

  star_fsh.source(
    "#version 150 core\n"
    "in float vf_zpos;\n"
    "in vec3 vf_color;\n"
    "out vec4 out_mi, out_hi;\n"

    "void main(void)\n"
    "{\n"
    "    out_mi = vec4(vec3(min(3.0 * vf_zpos, 1.0)) * vf_color, 1.0);\n"
    "    out_hi = vec4(0.0, 0.0, 0.0, 1.0);\n"
    "}"
  );

  if (!star_vsh.compile() || !star_fsh.compile()) {
    throw std::runtime_error("Could not compile star shaders");
  }

  star_prg = new gl::program;

  *star_prg << star_vsh;
  *star_prg << star_fsh;

  star_prg->bind_attrib("in_pos", 0);
  star_prg->bind_attrib("in_color", 1);

  star_prg->bind_frag("out_mi", 0);
  star_prg->bind_frag("out_hi", 1);

  if (!star_prg->link()) {
    throw std::runtime_error("Could not link star program");
  }


  fb_vertices = new gl::vertex_array;
  fb_vertices->set_elements(4);

  vec2 fb_vertex_positions[] = {
    vec2(-1.f, 1.f), vec2(-1.f, -1.f), vec2(1.f, 1.f), vec2(1.f, -1.f)
  };

  gl::vertex_attrib *va_p = fb_vertices->attrib(0);
  va_p->format(2);
  va_p->data(fb_vertex_positions);
  va_p->load();


  stars = new gl::vertex_array;
  stars->set_elements(STAR_COUNT);

  vec3 *star_positions = new vec3[STAR_COUNT], *star_colors = new vec3[STAR_COUNT];

  std::default_random_engine rng(std::chrono::system_clock::now().time_since_epoch().count());
  std::uniform_real_distribution<float> rng_dist(0.f, 1.f);

  for (int i = 0; i < STAR_COUNT; i++) {
    star_positions[i].x() = (rng_dist(rng) - .5f) * 20.f;
    star_positions[i].y() = (rng_dist(rng) - .5f) * 20.f;
    star_positions[i].z() = (rng_dist(rng) - .5f) * 10.f;

    float redness = rng_dist(rng);
    float min_brightness = rng_dist(rng) / 2.f + .5f;

    star_colors[i].r() = min(min_brightness + redness, 1.f);
    star_colors[i].b() = min(min_brightness + 1.f - redness, 1.f);
    star_colors[i].g() = min(star_colors[i].r(), star_colors[i].b());
  }

  va_p = stars->attrib(0);
  va_p->format(3);
  va_p->data(star_positions);
  va_p->load();

  gl::vertex_attrib *va_c = stars->attrib(1);
  va_c->format(3);;
  va_c->data(star_colors);
  va_c->load();

  delete[] star_positions;
  delete[] star_colors;
}

std::shared_ptr< ::model::Game const > const& GlRenderer::game_model() const
{
  return _game_model;
}

GlRenderer::delegate_factory_type& GlRenderer::drawable_factory()
{
  return _drawable_factory;
}

GlRenderer::delegate_factory_type const& GlRenderer::drawable_factory() const
{
  return _drawable_factory;
}

void GlRenderer::visualize_model( GlutWindow& w )
{
  fb->bind();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  cam = mat4::identity().translated(vec3(0.f, 0.f, -5.f));


  star_prg->use();
  star_prg->uniform<mat4>("mvp") = proj * cam;
  star_prg->uniform<float>("zdiff") = (std::chrono::duration_cast<std::chrono::milliseconds>(game_model()->timestamp().time_since_epoch()).count() % 4000) / 400.f;


  stars->draw(GL_POINTS);


  for (const auto &obj: game_model()->objects()) {
    auto delegated = obj->getData<Drawable>();
    if (delegated) {
      delegated->visualize(*this, w);
    }
  }


  const gl::texture *input_tex = &(*fb)[1];
  for (int i = 0, cur_fb = 0; i < 8; i++, cur_fb ^= 1) {
    blur_prg[cur_fb]->use();
    blur_fbs[cur_fb]->bind();

    input_tex->bind();
    blur_prg[cur_fb]->uniform<gl::texture>("input_tex") = *input_tex;
    blur_prg[cur_fb]->uniform<float>("epsilon") = exp2((8 - i / 2) / 2.f) / (cur_fb ? height : width);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    fb_vertices->draw(GL_TRIANGLE_STRIP);

    input_tex = &(*blur_fbs[cur_fb])[0];
  }


  gl::framebuffer::unbind();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  fb_prg->use();

  (*fb)[0].bind();
  input_tex->bind();

  fb_prg->uniform<gl::texture>("fb_mi") = (*fb)[0];
  fb_prg->uniform<gl::texture>("fb_hi") = *input_tex;

  fb_vertices->draw(GL_TRIANGLE_STRIP);

  glutSwapBuffers();
}

void GlRenderer::resize( GlutWindow& win ) 
{
  width = win.width();
  height = win.height();

  gl::framebuffer::unbind();
  glViewport(0, 0, width, height);

  for (gl::framebuffer *fbo: {fb, blur_fbs[0], blur_fbs[1]}) {
    fbo->bind();
    fbo->resize(width, height);
    glViewport(0, 0, width, height);
  }

  proj = mat4::projection(static_cast<float>(M_PI) / 2.f, static_cast<float>(width) / height, .5f, 100.f);
}
