#include <memory>

#include "flappy_box/model/box.hpp"
#include "flappy_box/controller/flappy_engine.hpp"
#include "controller/logic.hpp"
#include "view/al_renderer.hpp"
#include "view/gl_renderer.hpp"


using namespace flappy_box::controller;
using namespace flappy_box::model;


FlappyEngine::FlappyEngine(void):
  GlutEngine()
{
}


// god i hate you for this :: nonsense
FlappyEngine::FlappyEngine(const std::shared_ptr<::controller::Logic> &l, const std::shared_ptr<::view::AlRenderer> &al, const std::shared_ptr<::view::GlRenderer> &gl):
  GlutEngine(l, al, gl)
{
}


void FlappyEngine::init(int &argc, char **argv)
{
  ::controller::GlutEngine::init(argc, argv);

  Box::register_delegated(*this);

  addGameObject(std::make_shared<Box>());
}


void FlappyEngine::run(void)
{
  ::controller::GlutEngine::run();
}
