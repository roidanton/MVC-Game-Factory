#include <cstdio>
#include "controller/engine.hpp"
#include "test43/model/test43_object.hpp"
#include "test43/controller/test43_object_logic.hpp"
#include "test43/view/test43_audible.hpp"
#include "test43/view/test43_drawable.hpp"


model::Test43Object::Test43Object(void):
  GameObject(false, "Test 4.3")
{
}

model::Test43Object::~Test43Object(void)
{
}


GAME_OBJECT_GENERATE_REGISTER_DELEGATED(model::Test43Object, controller::Test43ObjectLogic, view::Test43Audible, view::Test43Drawable)
