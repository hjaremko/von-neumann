cmake_minimum_required(VERSION 3.10)
project(von-neumann)

set(TARGET_SOURCES
        parsing/cli_parser.cpp
        word.cpp
        machine.cpp
        parsing/error_reporter.cpp
        parsing/error.cpp
        parsing/interpreter.cpp
        parsing/line_interpreter.cpp
        parsing/scanner.cpp
        printing/normal.cpp
        printing/with_sign.cpp
        printing/binary.cpp
        )

add_executable(${PROJECT_NAME}
        main.cpp
        ${TARGET_SOURCES}
        )

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
