#ifndef TEST43_AUDIBLE_HPP
#define TEST43_AUDIBLE_HPP

#include "view/al_renderer.hpp"

namespace view
{

struct Test43Audible: public AlRenderer::Audible {
  virtual void auralize(AlRenderer &rnd);
};

}

#endif
