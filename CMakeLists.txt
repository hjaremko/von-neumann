cmake_minimum_required( VERSION 2.8.11 )

project( von-neumann )

set( CMAKE_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src )
set( CMAKE_BINARY_DIR ${PROJECT_SOURCE_DIR} )
set( EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR} )
set( LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR} )

include_directories( "${PROJECT_SOURCE_DIR}" )
link_libraries( stdc++fs )

if ( "${CMAKE_CXX_COMPILER_ID}" MATCHES "(GNU|Clang)" )
    set( CMAKE_CXX_FLAGS "-std=c++17 -O2 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wpedantic -Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual" )
endif()

set( TARGET_SOURCES
    ${CMAKE_SOURCE_DIR}/instruction_mappings.cpp
    ${CMAKE_SOURCE_DIR}/main.cpp
    ${CMAKE_SOURCE_DIR}/word.cpp
    ${CMAKE_SOURCE_DIR}/interpreter.cpp
    ${CMAKE_SOURCE_DIR}/machine.cpp
    ${CMAKE_SOURCE_DIR}/memory.cpp
    ${CMAKE_SOURCE_DIR}/scanner.cpp
)

add_executable( ${PROJECT_NAME} ${TARGET_SOURCES} )
