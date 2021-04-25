cmake_minimum_required(VERSION 3.13)
project(sqlite C)

set(CMAKE_C_STANDARD 99)

add_executable(sqlite main.c input.c input.h)