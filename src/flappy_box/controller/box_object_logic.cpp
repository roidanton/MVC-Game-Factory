#include <cmath>
#include <chrono>

#include "math.hpp"
#include "flappy_box/model/box.hpp"
#include "flappy_box/controller/box_object_logic.hpp"


using namespace flappy_box::model;
using namespace flappy_box::controller;


bool BoxObjectLogic::advance(::controller::Logic &logic, const ::controller::InputEventHandler::keyboard_event &evt)
{
  Box &box = dynamic_cast<Box &>(*obj);
  float step = static_cast<float>(logic.game_model()->timestep().count());

  // YOLO
  box.acceleration() /= exp(4.2f * step); // exponential decrease

  if (evt.key == 'w') {
    box.acceleration() = vec3_type(0.f, 0.f, 20.f);
  } else if (evt.key == 'q') {
    box.type() = !box.type();
  }

  vec3_type real_accel = box.acceleration() + vec3_type(0.f, 0.f, -9.81f); // who in their right mind uses Z as the vertical axis

  box.position() += box.speed() * step;
  box.speed()    += real_accel  * step;

  box.angle() += step * 360.f / 5.f;
  if (box.angle() >= 360.f) {
    box.angle() -= 360.f;
  }

  return true;
}
