cmake_minimum_required(VERSION 3.0)
project(SimpleRTS CXX)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp)

add_executable(SimpleRTS ${SOURCE_FILES})


if(MSVC)
	target_compile_options(SimpleRTS PRIVATE /W4)
endif()