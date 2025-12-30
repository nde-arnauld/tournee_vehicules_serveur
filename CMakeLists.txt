cmake_minimum_required(VERSION 4.1)
project(serveur)
include_directories(include)

set(CMAKE_CXX_STANDARD 11)

add_executable(serveur
        src/TestServeur.cpp
        src/utils/Socket.cpp
        src/utils/ServerSocket.cpp)
