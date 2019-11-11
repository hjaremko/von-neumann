cmake_minimum_required(VERSION 3.12)
project(von-neumann)

add_executable(${PROJECT_NAME}
        main.cpp
        ${TARGET_SOURCES}
        )

if (NOT MSVC)
    target_link_libraries(${PROJECT_NAME} PRIVATE project_warnings stdc++fs)
else ()
    target_link_libraries(${PROJECT_NAME} PRIVATE project_warnings)
endif ()

