cmake_minimum_required(VERSION 2.8)
project(cg)

include(local.cmake)
include(cmake/utils.cmake)

add_definitions(
  -std=c++11
  -Wall
  -pedantic
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets)
find_package(Qt5Core)

add_subdirectory(utils)
add_subdirectory(model)
add_subdirectory(controller)
add_subdirectory(gui)
add_subdirectory(app)

GENERATE_COMPLETION()
