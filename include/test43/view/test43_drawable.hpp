#ifndef TEST43_DRAWABLE_HPP
#define TEST43_DRAWABLE_HPP

#include "view/gl_renderer.hpp"

namespace view
{

struct Test43Drawable: public GlRenderer::Drawable {
  virtual void visualize(GlRenderer &, GlutWindow &);
};

}

#endif
