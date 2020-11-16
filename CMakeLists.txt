cmake_minimum_required(VERSION 3.10)

project(Demo)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED Ture)

include_directories(./include/)
link_directories(./lib/glfw/)

aux_source_directory(lib/glad/ glad)
add_library(libglad ${glad})

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} libglfw.3.dylib libglad)

