cmake_minimum_required(VERSION 3.6)
project(LexicalParser)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Parser.cpp Parser.h GlobalConst.h)
add_executable(LexicalParser ${SOURCE_FILES})