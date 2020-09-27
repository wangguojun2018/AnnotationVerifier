cmake_minimum_required(VERSION 2.8)

project(AnnotationVerifier)

find_package(PCL REQUIRED)

set(CMAKE_CXX_FLAGS "-std=c++11 -march=native")
#SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_BUILD_TYPE "Release")
SET(CMAKE_CXX_FLAGS_DEBUG  "-O0 -Wall -g -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE  "-O3 -Wall")


include_directories(${PCL_INCLUDE_DIRS} .)

add_library(utils "utils.h" "utils.cpp")

target_link_libraries(utils ${PCL_LIBRARIES})

add_executable(bbox_verifier "bbox_verifier.cpp")

target_link_libraries(bbox_verifier ${PCL_LIBRARIES} utils)


add_executable(semantic_verifier "semantic_verifier.cpp")

target_link_libraries(semantic_verifier ${PCL_LIBRARIES} utils)
