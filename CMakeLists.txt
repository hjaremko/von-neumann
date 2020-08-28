cmake_minimum_required(VERSION 3.10)
project(von-neumann LANGUAGES CXX VERSION 0.2.0)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

string(
        APPEND _opts
        "$<IF:$<CXX_COMPILER_ID:MSVC>,"
        "/W4;$<$<CONFIG:RELEASE>:/O2>,"
        "-Wall;-Wextra;-pedantic;-Werror;"
        "$<$<CONFIG:RELEASE>:-O3>"
        "$<$<CXX_COMPILER_ID:Clang>:-stdlib=libc++>"
        ">"
)

add_compile_options("${_opts}")

set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(FMT_SOURCE_DIR ${PROJECT_SOURCE_DIR}/extern/src/fmt)

include_directories("include")
include_directories(SYSTEM "extern/include")

add_subdirectory(src)
add_subdirectory(test)
