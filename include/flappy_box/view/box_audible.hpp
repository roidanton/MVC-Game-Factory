#ifndef FLAPPY_BOX__BOX_AUDIBLE_HPP
#define FLAPPY_BOX__BOX_AUDIBLE_HPP

#include <AL/al.h>

#include "view/al_renderer.hpp"

namespace flappy_box
{

namespace view
{

struct BoxAudible: public ::view::AlRenderer::Audible {
  BoxAudible(void);
  virtual ~BoxAudible(void);

  virtual void auralize(::view::AlRenderer &rnd);

  // god why would you ever use doubles are you from /b/ or what
  double base_frequency = 400., target_frequency = 100.;

  // You do realize that underscore prefixes are considered reserved?
  ALuint _al_box_sources[2];
  ALuint _al_sine_buffers[2];
};

}

}

#endif
