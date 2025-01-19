cmake_minimum_required(VERSION 3.13)  # CMake version check
project(chessengine)               # Create project "simple_example"
set(CMAKE_CXX_STANDARD 17)            # Enable c++17 standard
set(CMAKE_CXX_FLAGS "-O3")

add_subdirectory("src")

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(chessboardconverter chess_board_data.cpp)

add_executable(chessstockfishmoveconverter chess_stockfish_move_converter.cpp)

target_include_directories(chessboardconverter PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(chessboardconverter chessai)

target_include_directories(chessstockfishmoveconverter PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(chessstockfishmoveconverter chessai)