cmake_minimum_required(VERSION 3.10)
project(von_neumann_test)

if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "--coverage")
endif ()

set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../extern/include/catch)
add_library(Catch2::Catch IMPORTED INTERFACE)
set_property(TARGET Catch2::Catch PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${CATCH_INCLUDE_DIR}")

set(TEST_TARGET_SOURCES
        ${SOURCE_DIR}/word.cpp
        ${SOURCE_DIR}/machine.cpp
        ${SOURCE_DIR}/parsing/error_reporter.cpp
        ${SOURCE_DIR}/parsing/error.cpp
        ${SOURCE_DIR}/parsing/interpreter.cpp
        ${SOURCE_DIR}/parsing/line_interpreter.cpp
        ${SOURCE_DIR}/parsing/scanner.cpp
        ${FMT_SOURCE_DIR}/format.cc
        ${FMT_SOURCE_DIR}/os.cc
        )

set(TEST_SOURCES
        ${TEST_TARGET_SOURCES}
        main.cpp
        machine_test.cpp
        word_test.cpp
        memory_test.cpp
        interpreter_test.cpp
        )

add_executable(${PROJECT_NAME} ${TEST_SOURCES})
target_link_libraries(${PROJECT_NAME} Catch2::Catch)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)