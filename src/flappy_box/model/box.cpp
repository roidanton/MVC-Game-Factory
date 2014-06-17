#include "controller/engine.hpp"
#include "flappy_box/model/box.hpp"
#include "flappy_box/controller/box_object_logic.hpp"
#include "flappy_box/view/box_audible.hpp"
#include "flappy_box/view/box_drawable.hpp"


using namespace flappy_box::model;
using namespace flappy_box::controller;
using namespace flappy_box::view;


Box::Box(void):
  GameObject(true, "Flappy Box")
{
}

Box::~Box(void)
{
}


GAME_OBJECT_GENERATE_REGISTER_DELEGATED(Box, BoxObjectLogic, BoxAudible, BoxDrawable)
