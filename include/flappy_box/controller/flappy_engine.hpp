#ifndef FLAPPY_BOX__FLAPPY_ENGINE_HPP
#define FLAPPY_BOX__FLAPPY_ENGINE_HPP

#include "controller/glut_engine.hpp"
#include "controller/logic.hpp"
#include "view/al_renderer.hpp"
#include "view/gl_renderer.hpp"

namespace flappy_box
{

namespace controller
{

class FlappyEngine: public ::controller::GlutEngine {
public:
  FlappyEngine(void);
  FlappyEngine(const std::shared_ptr<::controller::Logic> &, const std::shared_ptr<::view::AlRenderer> &, const std::shared_ptr<::view::GlRenderer> &);

  virtual void init(int &argc, char **argv) final override;
  virtual void run(void) final override;
};

}

}

#endif
