project(nclist)
cmake_minimum_required(VERSION 2.8)
#set(CMAKE_VERBOSE_MAKEFILE ON)
#set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# Configuration options
set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel.")

message(STATUS "Build configuration: ${CMAKE_BUILD_TYPE}")
set(CXX_FLAGS               "-std=c++14")
set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} ${CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Werror -pedantic-errors")

set(SOURCES
    src/main.cpp
    src/nclist.cpp
)

add_executable(nclist ${SOURCES})

# Make all relevant files show up in the IDE
FILE(GLOB_RECURSE IDE_FILES "src/*")
add_custom_target(ide SOURCES ${IDE_FILES})
