cmake_minimum_required(VERSION 3.10)
project(von_neumann_test)

if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "--coverage")
endif ()

set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../extern/catch)
add_library(Catch2::Catch IMPORTED INTERFACE)
set_property(TARGET Catch2::Catch PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${CATCH_INCLUDE_DIR}")

set(TEST_SOURCES
        ${TARGET_SOURCES}
        main.cpp
        machine_test.cpp
        word_test.cpp
        memory_test.cpp
        )

add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests Catch2::Catch)