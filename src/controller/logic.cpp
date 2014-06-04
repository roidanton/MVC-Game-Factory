# include "controller/logic.hpp"

using namespace ::controller;

Logic::Logic( const std::shared_ptr< ::model::Game >& g ) : _model( g ) {}

bool Logic::advance_model( const ::controller::InputEventHandler::keyboard_event& ev )
{
  // TODO 4.3: set current timestamp in model and call delegates

  // i hate myself for not using the attributes themselves
  game_model()->setTimestamp(std::chrono::steady_clock::now());

  for (const auto &obj: game_model()->objects()) {
    auto delegated = obj->getData<ObjectLogic>();
    if (delegated) {
      delegated->advance(*this, ev);
    }
  }

  return false;
}
