# pragma once

#include <memory>

#include <AL/al.h>

#include "model/game.hpp"
#include "model/game_object.hpp"

# include "factory_map.hpp"

namespace view 
{
  class AlRenderer
  {
    public:
      struct Audible : public ::model::GameObject::Data
      {
        virtual void auralize( AlRenderer& ) = 0;
      };

      typedef factory_map< model::GameObject, Audible > delegate_factory_type;

      AlRenderer() = delete;
      AlRenderer( std::shared_ptr< model::Game const > const& );
      ~AlRenderer(void);

      void init_with_context(void);

      std::shared_ptr< model::Game const > const& game_model() const;

      /// Return factory creating Audible delegates.
      delegate_factory_type&       audible_factory();
      delegate_factory_type const& audible_factory() const;

      void auralize_model();

    private:
      std::string _name;
      std::shared_ptr< model::Game const > _game_model;
      delegate_factory_type _audible_factory;

      ALuint bg_source = AL_NONE;
      ALuint bg_buffer;

  }; // AlRenderer

} // view::

