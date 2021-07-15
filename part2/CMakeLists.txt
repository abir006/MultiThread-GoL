# cmake_minimum_required(VERSION <specify CMake version here>)
project(game_ex)

set(CMAKE_CXX_STANDARD 11)

SET(CMAKE_CXX_FLAGS  "-pthread")

add_executable(game_ex main.cpp Semaphore.cpp PCQueue.cpp game.cpp utils.cpp)