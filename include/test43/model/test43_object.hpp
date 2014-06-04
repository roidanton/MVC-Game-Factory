#ifndef TEST43_OBJECT_HPP
#define TEST43_OBJECT_HPP

#include "model/game_object.hpp"
#include "controller/engine.hpp"

namespace model
{

class Test43Object: public GameObject {
public:
  Test43Object(void);
  virtual ~Test43Object(void);

  static void register_delegated(controller::Engine &eng);
};

}

#endif
