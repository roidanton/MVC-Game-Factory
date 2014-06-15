#ifndef FLAPPY_BOX__BOX_HPP
#define FLAPPY_BOX__BOX_HPP

#include "math.hpp"

#include "model/game_object.hpp"
#include "controller/engine.hpp"

// I'm not so sure about this namespace organisation, but it's what the task
// says to do

namespace flappy_box
{

namespace model
{

// lol now I know why you would make the namespace hierarchy as broken as it is
// (so we *HAVE* to use :: as a prefix)
// ((we have to suffer just because you had to work at IBM? okay))
// (´；ω；｀)ﾌﾞﾜｯ
class Box: public ::model::GameObject {
public:
  Box(void);
  virtual ~Box(void);

  static void register_delegated(::controller::Engine &eng);

  vec3_type &position(void) { return s; }
  const vec3_type &position(void) const { return s; }

  vec3_type &speed(void) { return v; }
  const vec3_type &speed(void) const { return v; }

  vec3_type &acceleration(void) { return a; }
  const vec3_type &acceleration(void) const { return a; }

  float &angle(void) { return phi; }
  const float &angle(void) const { return phi; }

  bool &type(void) { return teapot; }
  const bool &type(void) const { return teapot; }


private:
  vec3_type s = vec3_type(0.f, 0.f, 0.f);
  vec3_type v = vec3_type(0.f, 0.f, 0.f);
  vec3_type a = vec3_type(0.f, 0.f, 0.f);
  float phi = 0.f; // phi is best greek letter
  bool teapot = false;
};

}

}

#endif
