cmake_minimum_required( VERSION 2.8 )

project(cpp4cg4)

include_directories( include )

add_definitions("-std=c++11 -g2 -O3")

# look for Cmake modules in project directory
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}")

#########################################################
# FIND OPENGL
#########################################################
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})

#########################################################
# FIND GLUT
#########################################################
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})

#########################################################
# FIND OPENAL
#########################################################
find_package(OpenAL REQUIRED)
include_directories(${OPENAL_INCLUDE_DIRS})
link_directories(${OPENAL_LIBRARY_DIRS})
add_definitions(${OPENAL_DEFINITIONS})

#########################################################
# FIND ALUT
#########################################################
 find_package(ALUT REQUIRED)
 include_directories(${ALUT_INCLUDE_DIRS})
 link_directories(${ALUT_LIBRARY_DIRS})
 add_definitions(${ALUT_DEFINITIONS})

#########################################################
# EXECUTABLE AUDIOphile
#########################################################

add_executable( ex41       src/main41.cpp
                           # model
                           src/model/game.cpp
                           src/model/game_object.cpp
                           # controller
                           src/controller/engine.cpp
                           src/controller/glut_engine.cpp
                           src/controller/logic.cpp
                           # view
                           src/view/glut_window.cpp
                           src/view/gl_renderer.cpp
                           src/view/al_renderer.cpp
                           # flappy_box
                           # src/flappy_box/model/box.cpp
                           # src/flappy_box/controller/flappy_engine.cpp 
                           # src/flappy_box/controller/box_object_logic.cpp 
                           # src/flappy_box/view/box_al_audible.cpp
                           # src/flappy_box/view/box_gl_drawable.cpp
              )
target_link_libraries( ex41 ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${OPENAL_LIBRARY} ${ALUT_LIBRARY} )

add_executable( ex43       src/main43.cpp
                           # model
                           src/model/game.cpp
                           src/model/game_object.cpp
                           # controller
                           src/controller/engine.cpp
                           src/controller/glut_engine.cpp
                           src/controller/logic.cpp
                           # view
                           src/view/glut_window.cpp
                           src/view/gl_renderer.cpp
                           src/view/al_renderer.cpp
                           # test43
                           src/test43/model/test43_object.cpp
                           src/test43/controller/test43_object_logic.cpp
                           src/test43/view/test43_audible.cpp
                           src/test43/view/test43_drawable.cpp
              )
target_link_libraries( ex43 ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${OPENAL_LIBRARY} ${ALUT_LIBRARY} )

add_executable( ex44       src/main44.cpp
                           # model
                           src/model/game.cpp
                           src/model/game_object.cpp
                           # controller
                           src/controller/engine.cpp
                           src/controller/glut_engine.cpp
                           src/controller/logic.cpp
                           # view
                           src/view/glut_window.cpp
                           src/view/gl_renderer.cpp
                           src/view/al_renderer.cpp
                           # flappy_box
                           src/flappy_box/model/box.cpp
                           src/flappy_box/controller/flappy_engine.cpp
                           src/flappy_box/controller/box_object_logic.cpp
                           src/flappy_box/view/box_audible.cpp
                           src/flappy_box/view/box_drawable.cpp
              )
target_link_libraries( ex44 ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${OPENAL_LIBRARY} ${ALUT_LIBRARY} )
