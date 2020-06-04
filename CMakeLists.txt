cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(SimpleRTS CXX)

set(CMAKE_CXX_STANDARD 11) 

file (GLOB_RECURSE sources src/*.cpp)
set(SOURCE_FILES ${sources})

add_executable(SimpleRTS ${SOURCE_FILES} )

#if you do not find the packages, consider using vcpkg
find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(SimpleRTS PRIVATE SDL2::SDL2 SDL2::SDL2main)

find_package(sdl2-image CONFIG REQUIRED)
target_link_libraries(SimpleRTS PRIVATE SDL2::SDL2_image)

find_package(sdl2-ttf CONFIG REQUIRED)
target_link_libraries(SimpleRTS PRIVATE SDL2::SDL2_ttf)

#find_package(sdl2-mixer CONFIG REQUIRED)
#target_link_libraries(SimpleRTS PRIVATE SDL2::SDL2_mixer)

include(FindLua)
find_package(Lua REQUIRED)
target_link_libraries(SimpleRTS PRIVATE ${LUA_LIBRARIES})
target_include_directories(SimpleRTS PRIVATE ${LUA_INCLUDE_DIR})


add_custom_command( TARGET SimpleRTS POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
				${CMAKE_SOURCE_DIR}/data/ $<TARGET_FILE_DIR:SimpleRTS>/data/)

