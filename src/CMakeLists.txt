cmake_minimum_required(VERSION 3.10)
project(von-neumann)

add_executable(${PROJECT_NAME}
        main.cpp
        ${TARGET_SOURCES}
        )

target_link_libraries(${PROJECT_NAME} PRIVATE project_warnings)
