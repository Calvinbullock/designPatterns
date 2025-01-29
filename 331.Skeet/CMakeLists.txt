cmake_minimum_required(VERSION 3.5)
project(SkeetShootingGame)

# Include directories for header files
include_directories(include) # Assuming headers are in 'include' directory

# Set source files
set(SOURCE_FILES
  bird.cpp
  bullet.cpp
  effect.cpp
  game.cpp
  gun.cpp
  points.cpp
  position.cpp
  score.cpp
  skeet.cpp
  time.cpp
  uiInteract.cpp
)

add_executable(SkeetShootingGame ${SOURCE_FILES})
target_link_libraries(SkeetShootingGame -lglut -lGL -lGLU -lGLEW)

