cmake_minimum_required(VERSION 3.1.0)

project(socket_test VERSION 0.1.0)

include(CTest)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_subdirectory(tests)
add_subdirectory(src/)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpic")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")


# sudo apt-get install libgsl-dev libgsl-dev libboost-dev
# libgtest-dev /usr/src/gtest make it
# sudo apt install libgnuplot-iostream-dev



