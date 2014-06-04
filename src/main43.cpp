#include <cstdio>
#include <memory>

#include "controller/glut_engine.hpp"
#include "test43/model/test43_object.hpp"

int main(int argc, char **argv)
{
  // TODO 4.3: test factory and delegates here
  // XXX clici: Actually, we are simply testing everything. Isn't that nice?

  std::shared_ptr<controller::GlutEngine> game_engine(new controller::GlutEngine);
  game_engine->init(argc, argv);

  model::Test43Object::register_delegated(*game_engine);

  std::shared_ptr<model::Test43Object> obj(new model::Test43Object);
  game_engine->addGameObject(obj);

  game_engine->run();

  return 0;
}
