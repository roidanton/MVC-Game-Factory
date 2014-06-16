#ifndef FLAPPY_BOX__BOX_DRAWABLE_HPP
#define FLAPPY_BOX__BOX_DRAWABLE_HPP

#include "view/gl_renderer.hpp"
#include "view/glut_window.hpp"

namespace flappy_box
{

namespace view
{

struct BoxDrawable: public ::view::GlRenderer::Drawable {
  BoxDrawable(void);

  virtual void visualize(::view::GlRenderer &, ::view::GlutWindow &);
};

}

}

#endif
