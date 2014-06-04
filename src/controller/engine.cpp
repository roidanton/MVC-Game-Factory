# include "controller/engine.hpp"

using namespace ::controller;

Engine::Engine(void)
{
  std::shared_ptr<model::Game> game = std::make_shared<model::Game>();

  _logic = std::make_shared<Logic>(game);
  _al = std::make_shared<view::AlRenderer>(game);
  _gl = std::make_shared<view::GlRenderer>(game);

  _model = _logic->game_model();
}

Engine::Engine(const std::shared_ptr<Logic>& l, const std::shared_ptr<view::AlRenderer> &al, const std::shared_ptr<view::GlRenderer> &gl):
  _model(l->game_model()), _logic(l), _al(al), _gl(gl)
{}

std::shared_ptr< ::model::Game >& Engine::game_model()
{
  return _model;
}

std::shared_ptr< ::model::Game const > const Engine::game_model() const
{
  return _model;
}

std::shared_ptr< Logic >& Engine::game_logic()
{
  return _logic;
}

std::shared_ptr< Logic const > const Engine::game_logic() const
{
  return _logic;
}

std::shared_ptr<view::AlRenderer> &Engine::al_renderer(void)
{
  return _al;
}

const std::shared_ptr< view::AlRenderer > Engine::al_renderer(void) const
{
  return _al;
}

std::shared_ptr<view::GlRenderer> &Engine::gl_renderer(void)
{
  return _gl;
}

const std::shared_ptr< view::GlRenderer > Engine::gl_renderer(void) const
{
  return _gl;
}

void Engine::step( InputEventHandler::keyboard_event const& ev )
{
  game_logic()->advance_model( ev );

  // TODO clici: why am i not hearing anything am i deaf? have factories
  // destroyed my sense of hearing? but how? oh no yes i need to call auralize
  //
  // Well, we ought to call auralize() somewhere regularily and it probably
  // makes most sense to call it after all objects have been advanced.
  al_renderer()->auralize_model();
}

