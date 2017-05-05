cmake_minimum_required(VERSION 3.6)
project(JWSearch)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp index.cpp index.h data.h data.cpp Parameters.h search.h search.cpp query.h query.cpp utility.h utility.cpp verify.h verify.cpp func.h func.cpp)
add_executable(JWSearch ${SOURCE_FILES})