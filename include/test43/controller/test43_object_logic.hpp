#ifndef TEST43_OBJECT_LOGIC_HPP
#define TEST43_OBJECT_LOGIC_HPP

#include "controller/input_event_handler.hpp"
#include "controller/logic.hpp"


namespace controller
{

struct Test43ObjectLogic: public Logic::ObjectLogic {
  virtual bool advance(Logic &, const InputEventHandler::keyboard_event &);
};

}

#endif
