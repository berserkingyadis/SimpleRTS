cmake_minimum_required(VERSION 3.0)
project(SimpleRTS CXX)

if (${CMAKE_SIZEOF_VOID_P} EQUAL "8")
    set (PROJECT_ARCHITECTURE "x64")
else ()
    set (PROJECT_ARCHITECTURE "x86")
endif ()

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})

add_executable(SimpleRTS ${SOURCE_FILES})
target_link_libraries(SimpleRTS ${SDL2_LIBRARY} )

if(MSVC)
	target_compile_options(SimpleRTS PRIVATE /W4)
endif()

