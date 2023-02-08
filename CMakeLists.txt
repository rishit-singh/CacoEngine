cmake_minimum_required(VERSION 2.8)
project(CacoEngine)

set(CMAKE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/opt/local/include/SDL2 -D_THREAD_SAFE --std=c++20")
file(GLOB CacoEngine_SOURCES 
		src/*.cpp)

find_package(SDL2 REQUIRED)

include_directories( ${SDL2_INCLUDE_DIRS} )
include_directories( include/ )
add_executable(CacoEngine ${CacoEngine_SOURCES})
#add_executable(CVcli src/main.cpp)
target_include_directories(CacoEngine PRIVATE include/)
target_link_libraries(CacoEngine ${SDL2_LIBRARIES})
