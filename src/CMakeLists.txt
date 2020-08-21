cmake_minimum_required(VERSION 3.10)
project(von-neumann)

set(TARGET_SOURCES
        word.cpp
        interpreter.cpp
        machine.cpp
        scanner.cpp
        cli_parser.cpp
        print/normal.cpp
        print/with_sign.cpp
        print/binary.cpp
        )

add_executable(${PROJECT_NAME}
        main.cpp
        ${TARGET_SOURCES}
        )

target_link_libraries(${PROJECT_NAME} PRIVATE project_warnings)
