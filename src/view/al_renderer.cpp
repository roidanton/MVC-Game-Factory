#include <AL/al.h>
#include <AL/alut.h>

#include <stdexcept>
#include <string>

# include "view/al_renderer.hpp"

using namespace ::view;

AlRenderer::AlRenderer( std::shared_ptr< model::Game const > const& g ) 
: _game_model( g ) 
{}

AlRenderer::~AlRenderer(void)
{
  if (bg_source != AL_NONE) {
    alDeleteBuffers(1, &bg_buffer);
    alDeleteSources(1, &bg_source);
  }
}

void AlRenderer::init_with_context(void)
{
  alGenSources(1, &bg_source);

  bg_buffer = alutCreateBufferFromFile("assets/bgm.wav");
  if (bg_buffer == AL_NONE) {
    throw std::runtime_error("Could not load background music; supported types: " + std::string(alutGetMIMETypes(ALUT_LOADER_BUFFER)));
  }

  alSource3f(bg_source, AL_POSITION, 0.f, 0.f, 5.f);;
  alSourcei(bg_source, AL_BUFFER, bg_buffer);
  alSourcei(bg_source, AL_LOOPING, true);

  alSourcePlay(bg_source);
}

std::shared_ptr< model::Game const > const& AlRenderer::game_model() const
{
  return _game_model;
}

AlRenderer::delegate_factory_type& AlRenderer::audible_factory()
{
  return _audible_factory;
}

AlRenderer::delegate_factory_type const& AlRenderer::audible_factory() const
{
  return _audible_factory;
}

void AlRenderer::auralize_model()
{
  for (const auto &obj: game_model()->objects()) {
    auto delegated = obj->getData<Audible>();
    if (delegated) {
      // +1 for "auralize"
      delegated->auralize(*this);
    }
  }
}
