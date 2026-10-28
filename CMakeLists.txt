cmake_minimum_required(VERSION 3.16)
project(milan_c VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_library(milan_core
    src/catalog.c
    src/cart.c
    src/app.c)
target_include_directories(milan_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
if(MSVC)
    target_compile_options(milan_core PRIVATE /W4)
else()
    target_compile_options(milan_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(milan src/main.c src/console.c)
target_link_libraries(milan PRIVATE milan_core)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

include(CTest)
if(BUILD_TESTING)
    add_executable(test_conformance tests/test_conformance.c)
    target_link_libraries(test_conformance PRIVATE milan_core)
    add_test(NAME conformance COMMAND test_conformance ${CMAKE_CURRENT_BINARY_DIR}/data/PRODUCTOS.csv)
    add_executable(test_catalog tests/test_catalog.c)
    target_link_libraries(test_catalog PRIVATE milan_core)
    add_test(NAME catalog COMMAND test_catalog ${CMAKE_CURRENT_SOURCE_DIR}/data/PRODUCTOS.csv)
endif()
