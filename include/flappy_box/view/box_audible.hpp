#ifndef FLAPPY_BOX__BOX_AUDIBLE_HPP
#define FLAPPY_BOX__BOX_AUDIBLE_HPP

#include "view/al_renderer.hpp"

namespace flappy_box
{

namespace view
{

struct BoxAudible: public ::view::AlRenderer::Audible {
  virtual void auralize(::view::AlRenderer &rnd);
};

}

}

#endif
