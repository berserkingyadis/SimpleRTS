cmake_minimum_required(VERSION 3.5)
project(SimpleRTS CXX)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp LTexture.cpp)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})

add_executable(SimpleRTS ${SOURCE_FILES})
target_link_libraries(SimpleRTS ${SDL2_LIBRARY}  ${SDL2_IMAGE_LIBRARIES})

if(MSVC)
	target_compile_options(SimpleRTS PRIVATE /W4)
endif()

