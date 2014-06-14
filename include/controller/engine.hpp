# pragma once

#include <memory>
#include <type_traits>

#include "model/game.hpp"
#include "model/game_object.hpp"
#include "controller/logic.hpp"
#include "view/al_renderer.hpp"
#include "view/gl_renderer.hpp"

namespace controller 
{
  class Engine: public controller::InputEventHandler, public std::enable_shared_from_this<controller::Engine>
  {
    public:
        Engine(void);
        Engine(const std::shared_ptr<Logic> &, const std::shared_ptr<view::AlRenderer> &, const std::shared_ptr<view::GlRenderer> &);

      virtual void init( int&, char** ) = 0;
      virtual void run() = 0;

      std::shared_ptr< model::Game >&             game_model();
      std::shared_ptr< model::Game const > const game_model() const;

      std::shared_ptr< Logic >&             game_logic();
      std::shared_ptr< Logic const > const game_logic() const;

      std::shared_ptr<view::AlRenderer> &al_renderer(void);
      const std::shared_ptr<view::AlRenderer> al_renderer(void) const;

      std::shared_ptr<view::GlRenderer> &gl_renderer(void);
      const std::shared_ptr<view::GlRenderer> gl_renderer(void) const;

      template<typename GameObject> void addGameObject(std::shared_ptr<GameObject> obj);

    protected:
      // Calls everything we need to advance the game.
      // Override this function to integrate new behavior and component that have to be called after the model was updated.
      virtual void step( InputEventHandler::keyboard_event const& ev = controller::InputEventHandler::keyboard_event() );

      // TODO 4.1: declare handle function DONE?
      // Implements abstract class InputEventHandler. Simply calls function step providing the keyboard_event ev as an argument.
      // Input events should be provided by some input-event-provider (e.g. GlutWindow).
      // Override this function to change event distribution and processing.
	  virtual bool handle(keyboard_event const& e) { step(e); return true; } // irgendwas zurückgeben, hier egal


    private:
      std::shared_ptr< model::Game > _model;
      std::shared_ptr< Logic >       _logic;
      std::shared_ptr<view::AlRenderer> _al;
      std::shared_ptr<view::GlRenderer> _gl;

  }; // Engine

} // controller::


template<typename GameObject> void controller::Engine::addGameObject(std::shared_ptr<GameObject> obj)
{
  static_assert(std::is_base_of<model::GameObject, GameObject>::value, "addGameObject() must be given a pointer to a subclass of model::GameObject");

  obj->registerData(game_logic()->logic_factory().create_for(obj));
  obj->registerData(al_renderer()->audible_factory().create_for(obj));
  obj->registerData(gl_renderer()->drawable_factory().create_for(obj));

  game_model()->addGameObject(obj);
}
