#ifndef FLAPPY_BOX__BOX_OBJECT_LOGIC_HPP
#define FLAPPY_BOX__BOX_OBJECT_LOGIC_HPP

#include "controller/input_event_handler.hpp"
#include "controller/logic.hpp"


namespace flappy_box
{

namespace controller
{

struct BoxObjectLogic: public ::controller::Logic::ObjectLogic {
  virtual bool advance(::controller::Logic &, const ::controller::InputEventHandler::keyboard_event &);
};

}

}

#endif
