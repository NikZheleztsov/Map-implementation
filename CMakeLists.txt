cmake_minimum_required(VERSION 3.20.3)
project(Map)

set(CMAKE_CXX_FLAGS "-g")
include_directories (include)
file(GLOB SOURCES "src/*.cpp")

add_executable(map ${SOURCES})
