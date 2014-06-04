#include "controller/glut_engine.hpp"
#include "view/glut_window.hpp"

#include <GL/freeglut.h>

using namespace ::controller;

// global file-scope function variable which can be reached from functions GlutEngine::init, GlutEngine::run and glutTimer.
// NOTE: This is ugly(destroys data encapsulation) but unavoidable!?
static std::function< void () > __controller__glut_engine__step_func = [](){ throw std::logic_error( "Default (undefined) function called in __controller__glut_engine__step_func." ); };

// Calls global step function first.
// Re-registering GLUT timer callback function.
void glutStepTimer( int interval )
{
  __controller__glut_engine__step_func();
  glutTimerFunc( interval, glutStepTimer, interval );
}

GlutEngine::GlutEngine(void): Engine()
{}

GlutEngine::GlutEngine(const std::shared_ptr<Logic> &l, const std::shared_ptr<view::AlRenderer> &al, const std::shared_ptr<view::GlRenderer> &gl)
: Engine(l, al, gl)
{}

void GlutEngine::init( int& argc, char** argv )
{
  glutInit( &argc, argv );
  glutSetOption( GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION );
  // Set global function which will be invoked by glutStepTimer.
  __controller__glut_engine__step_func = [this](){ this->step(  ); };
  // Register glutStepTimer for the first time, which re-registers itself until the glutMainLoop finishes.
  glutTimerFunc( _prefered_timestep_millisec, glutStepTimer, _prefered_timestep_millisec );
}

void GlutEngine::run()
{
  std::shared_ptr<view::GlutWindow> wnd(new view::GlutWindow("ossu sekai ni", 800, 600, gl_renderer(), shared_from_this()));

  // Run game.
  glutMainLoop();
}


