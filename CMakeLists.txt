cmake_minimum_required(VERSION 3.13)  # CMake version check
project(chessengine)               # Create project "simple_example"
set(CMAKE_CXX_STANDARD 17)            # Enable c++17 standard
set(CMAKE_CXX_FLAGS "-O3")

add_subdirectory("src")

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(chessengine chess_board_data.cpp)

target_include_directories(chessengine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(chessengine chessai)