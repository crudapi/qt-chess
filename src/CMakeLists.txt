cmake_minimum_required(VERSION 3.20)
project(CrChessQt)

include_directories("chess_engine")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
 
add_subdirectory(chess_engine)
add_subdirectory(fen)
add_subdirectory(chess_window)
add_subdirectory(cpplog)
