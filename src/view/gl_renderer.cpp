#include <dake/gl/gl.hpp>
#include <dake/math/matrix.hpp>

# include "view/gl_renderer.hpp"
# include "view/glut_window.hpp"

# include "GL/freeglut.h"

using namespace ::view;
using namespace dake::math;

GlRenderer::GlRenderer( std::shared_ptr< model::Game const > const& g )
: _game_model( g )
{}

void GlRenderer::init_with_context(void)
{
  glClearColor(.1f, .2f, .3f, 1.f);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
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
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  cam = mat4::identity().translated(vec3(0.f, 0.f, -7.f));

  for (const auto &obj: game_model()->objects()) {
    auto delegated = obj->getData<Drawable>();
    if (delegated) {
      delegated->visualize(*this, w);
    }
  }

  glutSwapBuffers();
}

void GlRenderer::resize( GlutWindow& win ) 
{
  glViewport(0, 0, win.width(), win.height());

  proj = mat4::projection(static_cast<float>(M_PI) / 2.f, static_cast<float>(win.width()) / win.height(), .5f, 100.f);
}
