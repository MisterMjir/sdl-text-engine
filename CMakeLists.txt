cmake_minimum_required(VERSION 3.10)

project(app)

add_executable(app
  src/main.cpp
  src/text_engine.cpp
)

target_include_directories(app PUBLIC
  src
)

target_link_libraries(app SDL2 SDL2_ttf)
set_target_properties(app PROPERTIES RUNTIME_OUTPUT_DIRECTORY bin)
