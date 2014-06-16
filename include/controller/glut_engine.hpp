# pragma once

#include <chrono>
#include <memory>

#include "controller/engine.hpp"
#include "view/glut_window.hpp"

namespace controller 
{

  class GlutEngine : public Engine
  {
    public:
        GlutEngine(void);
        GlutEngine(const std::shared_ptr<Logic> &, const std::shared_ptr<view::AlRenderer> &, const std::shared_ptr<view::GlRenderer> &);

      virtual void init( int& argc, char** argv ) override;
      virtual void run() override;
    protected:
      // time interval in which the (virtual) Engine::step function should be called.
      unsigned int _prefered_timestep_millisec = 30;
      std::shared_ptr<view::GlutWindow> wnd;

  }; // GlutEngine

} // controller::
