cmake_minimum_required (VERSION 3.8)

project ("OpenGL Mandelbrot")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory("lib")
add_subdirectory("shaders")
add_subdirectory("textures")
add_subdirectory ("src")
