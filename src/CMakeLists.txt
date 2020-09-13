cmake_minimum_required(VERSION 3.8)

add_executable(openglmandelbrot "main.cpp" "shader.cpp" "shader.h" "buffer.h")
target_link_libraries(openglmandelbrot PRIVATE glad)
target_link_libraries(openglmandelbrot PRIVATE glfw)
