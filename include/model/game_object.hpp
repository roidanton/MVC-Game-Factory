# pragma once

#include <cassert>
#include <memory>
#include <string>
#include <iostream>
#include <map>
#include <typeindex>
#include <type_traits>

namespace model
{
  class Game;

  class GameObject: public std::enable_shared_from_this<GameObject>
  {
    public:
      GameObject(bool dynamic_flag = true, const std::string &name = "untitled GameObject");
      // we need at least one virtual function to be polymorphic.
      virtual ~GameObject() { }

      struct Data
      {
        virtual ~Data() {}

        // Backlink
        std::shared_ptr<model::GameObject> obj;
      };

      template< typename DataType >
      void registerData( const std::shared_ptr< DataType >& );

      template< typename DataType >
      std::shared_ptr< DataType > getData();

      // dynamic flag can be used to mark object as static/dynamic in the game.
      // (this can be used as a hint to the game logic, indicating if object needs to be "advanced")
      bool is_dynamic() const { return _dynamic_flag; }

      const std::string& name() const { return _name; }

    private:

      bool _dynamic_flag;
      std::map< std::type_index, std::shared_ptr< Data > > _data; 
      std::string _name;

  }; // GameObject

} // model::


// I know you don't like macros, but it's so much nicer this way
#define GAME_OBJECT_GENERATE_REGISTER_DELEGATED(object, logic, audible, drawable) \
  void object::register_delegated(::controller::Engine &eng) \
  { \
    eng.game_logic()->logic_factory().register_module<object>( \
      [](const std::shared_ptr<object> &obj) { std::shared_ptr<logic> ptr(new logic); ptr->obj = obj; return ptr; } \
    ); \
    eng.al_renderer()->audible_factory().register_module<object>( \
      [](const std::shared_ptr<object> &obj) { std::shared_ptr<audible> ptr(new audible); ptr->obj = obj; return ptr; } \
    ); \
    eng.gl_renderer()->drawable_factory().register_module<object>( \
      [](const std::shared_ptr<object> &obj) { std::shared_ptr<drawable> ptr(new drawable); ptr->obj = obj; return ptr; } \
    ); \
  }

// Also, these macros are a protest against overengineering (*cough* factories
// *cough* in C++).


// implementation //

template< typename DataType >
void model::GameObject::registerData( const std::shared_ptr< DataType >& d )
{
  static_assert( std::is_base_of< Data, DataType >::value, "DataType must derive from model::GameObject::Data." );

  if( _data.find( typeid( DataType ) ) == _data.end() )
    _data.insert( { typeid( DataType ), d } );
  else if( typeid( _data[ typeid( DataType ) ] ) != typeid( d ) )
    throw std::logic_error( "::model::GameObject::registerDataType: Data for same key but with different type was already added." );
  else
    std::clog << "::model::GameObject::registerDataType: " << "Warning: Data for same key and same type was already added." << std::endl;
}

template< typename DataType >
std::shared_ptr< DataType > model::GameObject::getData()
{
  auto found = _data.find( typeid( DataType ) );

  if( found == _data.end() )
  { std::clog << "::model::GameObject::getData: Could not find data of type \"" << typeid( DataType ).name() << "\"." << std::endl;
    return { nullptr };
  }

  auto data_type_ptr = std::dynamic_pointer_cast< DataType >( found->second );
  if( ! data_type_ptr )
    std::clog << "::model::GameObject::getData: Could not cast to \"" << typeid( DataType ).name() << "\"." << std::endl;

  return data_type_ptr;
}
