cmake_minimum_required(VERSION 3.10)
project(von-neumann)

set(TARGET_SOURCES
        cli_parser.cpp
        word.cpp
        machine.cpp
        error_reporter.cpp
        interpreter.cpp
        scanner.cpp
        print/normal.cpp
        print/with_sign.cpp
        print/binary.cpp
        )

add_executable(${PROJECT_NAME}
        main.cpp
        ${TARGET_SOURCES}
        )

target_link_libraries(${PROJECT_NAME} PRIVATE project_warnings)
