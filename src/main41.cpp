# include "controller/glut_engine.hpp"

int main( int argc, char** argv )
{
  std::shared_ptr< ::controller::Engine > game_engine( new ::controller::GlutEngine );

  game_engine->init( argc, argv );
  game_engine->run();

  return 0;
}
