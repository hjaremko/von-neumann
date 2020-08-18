cmake_minimum_required(VERSION 3.10)
project(von-neumann LANGUAGES CXX VERSION 0.1.2)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

include_directories("include")
include_directories(SYSTEM "extern/catch")
include_directories(SYSTEM "extern/cxxopts")

add_library(project_warnings INTERFACE)

if (MSVC)
    target_compile_options(project_warnings INTERFACE /W4)
else ()
    target_compile_options(project_warnings
            INTERFACE
#            -Werror
            -Wall
            -Wextra
            -Wshadow
            -Wnon-virtual-dtor
            -Wold-style-cast
            -Wcast-align
            -Wunused
            -Woverloaded-virtual
            -Wpedantic
            -Wconversion
            -Wsign-conversion
            -Wmisleading-indentation
            -Wduplicated-cond
            -Wduplicated-branches
            -Wlogical-op
            -Wnull-dereference
            -Wuseless-cast
            -Wdouble-promotion
            -Wformat=2
            )
endif ()

set(TARGET_SOURCES
        ${CMAKE_SOURCE_DIR}/word.cpp
        ${CMAKE_SOURCE_DIR}/interpreter.cpp
        ${CMAKE_SOURCE_DIR}/machine.cpp
        ${CMAKE_SOURCE_DIR}/memory.cpp
        ${CMAKE_SOURCE_DIR}/scanner.cpp
        )

add_subdirectory(src)
add_subdirectory(test)
