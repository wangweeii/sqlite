cmake_minimum_required(VERSION 3.18)
project(sqlite)

set(CMAKE_CXX_STANDARD 14)

add_executable(sqlite main.cc input.cc input.h row.cc row.h exec.cc exec.h parser.cc parser.h table.cc table.h)