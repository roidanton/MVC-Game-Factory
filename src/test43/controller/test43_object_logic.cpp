#include <cstdio>
#include "test43/controller/test43_object_logic.hpp"


bool controller::Test43ObjectLogic::advance(controller::Logic &, const controller::InputEventHandler::keyboard_event &)
{
  puts("To infinity and beyond!");

  return true;
}
