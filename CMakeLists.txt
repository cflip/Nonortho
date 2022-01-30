cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 17)

# Give me warnings as much as possible
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wnon-virtual-dtor -pedantic -Wcast-align -Wnull-dereference")

project(Nonortho)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(third-party/sdl2)

add_subdirectory(src)
