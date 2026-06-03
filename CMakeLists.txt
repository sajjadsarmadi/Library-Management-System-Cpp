cmake_minimum_required(VERSION 3.15)
project(LibraryManagementSystem VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES 
    "src/*.cpp"
    "src/*.h"
    "*.cpp"
    "*.h"
)

add_executable(LibraryManagementSystem ${SOURCES})
