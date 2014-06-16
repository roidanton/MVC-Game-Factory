# pragma once

#include <memory>
#include <dake/gl/framebuffer.hpp>
#include <dake/gl/shader.hpp>
#include <dake/math/matrix.hpp>

#include "model/game.hpp"
#include "model/game_object.hpp"

# include "factory_map.hpp"

namespace view 
{
  class GlutWindow;

  class GlRenderer
  {
    public:
      struct Drawable : public ::model::GameObject::Data
      {
        virtual void visualize( GlRenderer&, GlutWindow& ) = 0;
      };

      typedef factory_map< model::GameObject, Drawable > delegate_factory_type;

      GlRenderer() = delete;
      GlRenderer( std::shared_ptr< model::Game const > const& );

      void init_with_context(void);

      std::shared_ptr< model::Game const > const& game_model() const;

      /// Return factory creating Drawable delegates.
      delegate_factory_type&       drawable_factory();
      delegate_factory_type const& drawable_factory() const;

      dake::math::mat4 &camera(void) { return cam; }
      const dake::math::mat4 &camera(void) const { return cam; }

      const dake::math::mat4 &projection(void) const { return proj; }

      virtual void visualize_model( GlutWindow& );
      virtual void resize( GlutWindow& );

    private:
      std::shared_ptr< model::Game const > _game_model;
      delegate_factory_type _drawable_factory;

      dake::math::mat4 cam, proj;
      dake::gl::framebuffer *fb, *blur_fbs[2];
      dake::gl::program *blur_prg[2];

      unsigned width, height;

  }; // GlRenderer

} // view::

