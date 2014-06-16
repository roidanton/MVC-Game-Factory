# pragma once

#include <dake/math/matrix.hpp>

# include <cmath>

typedef double distance_type;

template<unsigned D, typename T>
using vec_type = dake::math::vec<D, T>;

typedef dake::math::vec3 vec3_type;
