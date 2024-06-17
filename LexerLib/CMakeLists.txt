cmake_minimum_required(VERSION 3.27)
project(LexerLib)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_library(LexerLib src/Lexer.cpp src/Graph.cpp)
target_include_directories(LexerLib PUBLIC include)