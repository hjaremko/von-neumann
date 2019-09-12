cmake_minimum_required( VERSION 3.10 )

project( von-neumann LANGUAGES CXX VERSION 0.0.1 )

set( CMAKE_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src )
set( EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin )
set( LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib )

set( CMAKE_CXX_STANDARD 17 )
include_directories( "include" )
include_directories( "extern/cxxopts" )

if ( "${CMAKE_CXX_COMPILER_ID}" MATCHES "(GNU|Clang)" )
    set( CMAKE_CXX_FLAGS "-O3 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wpedantic -Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual" )
endif()

set( TARGET_SOURCES
    ${CMAKE_SOURCE_DIR}/instruction_mappings.cpp
    ${CMAKE_SOURCE_DIR}/word.cpp
    ${CMAKE_SOURCE_DIR}/interpreter.cpp
    ${CMAKE_SOURCE_DIR}/machine.cpp
    ${CMAKE_SOURCE_DIR}/memory.cpp
    ${CMAKE_SOURCE_DIR}/scanner.cpp
)

add_executable( ${PROJECT_NAME}
        ${CMAKE_SOURCE_DIR}/main.cpp
        ${TARGET_SOURCES}
    )

if ( NOT MSVC )
    target_link_libraries( ${PROJECT_NAME} stdc++fs )
endif()

add_subdirectory( test )
