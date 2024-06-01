cmake_minimum_required(VERSION 3.20)
project(CrChessQt)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")

include_directories("chess_engine")
include_directories("chess_play")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
 
add_subdirectory(chess_engine)
add_subdirectory(fen)
add_subdirectory(chess_play)
add_subdirectory(chess_window)
