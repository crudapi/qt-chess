cmake_minimum_required(VERSION 3.20)
project(chess_play)
add_library(chess_play CrChessPlay.cpp)
target_link_libraries(chess_play chess_engine)
