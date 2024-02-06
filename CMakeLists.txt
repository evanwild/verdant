cmake_minimum_required(VERSION 3.28)

project(Verdant)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(Verdant
    compiler/codegen.cpp
    compiler/lexer.cpp
    compiler/main.cpp
    compiler/parser.cpp
    compiler/semantic.cpp
)
