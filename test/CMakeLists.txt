project( von_neumann_test )

set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../extern/catch)
add_library(Catch2::Catch IMPORTED INTERFACE)
set_property(TARGET Catch2::Catch PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${CATCH_INCLUDE_DIR}")

#target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

set(TEST_SOURCES
        ${TARGET_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/machine_test.cpp
    )

add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests Catch2::Catch)