cmake_minimum_required(VERSION 3.5)

project(Ant VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories(
  ${CMAKE_SOURCE_DIR}/ant
)

set(HEADERS
  model/ant_algorithm.h
  model/graph.h
  model/heuristics.h
  view/console.h
)

set(SOURCES
  model/ant_algorithm.cc
  model/graph.cc
  view/console.cc
  main.cc
)

add_executable(
  ${PROJECT_NAME}
  ${HEADERS}
  ${SOURCES}
)

target_compile_options(
    ${PROJECT_NAME}
    PRIVATE
    -Wall
    -Werror
    -Wextra
    -std=c++17
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    -pthread
)

target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
