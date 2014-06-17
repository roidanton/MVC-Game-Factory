# include "view/gl_renderer.hpp"

# include "view/glut_window.hpp"

# include "GL/freeglut.h"

using namespace ::view;

GlRenderer::GlRenderer( std::shared_ptr< model::Game const > const& g )
: _game_model( g )
{}

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
  // TODO 4.3: initialize OpenGL context, call delegates and swap buffers

  // I personally would not call glClearColor() here; I'm just following orders, though
  glClearColor(.1f, .2f, .3f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // lol glMatrixMode, what is this, 1999?
  glMatrixMode(GL_MODELVIEW);
  // I'm embarrassed, to say the least
  glLoadIdentity();

  // lol using Z as the up vector
  // pleb
  gluLookAt(0., -7., 0., 0., 0., 0., 0., 0., 1.);

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
  glViewport( 0,0, win.width(), win.height() );
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity(); //Reset the camera
  gluPerspective( 45., win.width() / double( win.height() ), .5, 100. );
}
