cmake_minimum_required(VERSION 3.13)
project(sqlite)

set(CMAKE_CXX_STANDARD 14)

add_executable(sqlite main.cc input.cc input.h cursor.cc cursor.h exec.cc exec.h parser.cc parser.h save.cc save.h node.cc node.h data.h)