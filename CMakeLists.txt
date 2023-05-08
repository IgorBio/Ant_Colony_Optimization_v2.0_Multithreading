cmake_minimum_required(VERSION 3.5)

project(Ant LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -Werror -Wextra" )
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/model
  ${CMAKE_SOURCE_DIR}/view
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

target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
