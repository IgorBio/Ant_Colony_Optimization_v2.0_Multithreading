cmake_minimum_required(VERSION 3.5)

project(Ant VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
  ${CMAKE_SOURCE_DIR}/ant
)

set(HEADERS
  ant_algorithm.h
  graph.h
  heuristics.h
  console.h
)

set(SOURCES
  ant_algorithm.cc
  graph.cc
  console.cc
  main.cc
)

add_executable(
  ${PROJECT_NAME}
  ${HEADERS}
  ${SOURCES}
)