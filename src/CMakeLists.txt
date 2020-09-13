cmake_minimum_required(VERSION 3.8)

set(HDRS
	"shader.h"
	"buffer.h"
	"vertex_array.h")

set(SRCS
	"shader.cpp"
	"buffer.inl"
	"vertex_array.cpp"
	"main.cpp")

add_executable(openglmandelbrot ${SRCS} ${HDRS})
target_link_libraries(openglmandelbrot PRIVATE glad)
target_link_libraries(openglmandelbrot PRIVATE glfw)
