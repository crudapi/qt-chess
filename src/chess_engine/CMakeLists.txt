cmake_minimum_required(VERSION 3.20)
project(chess_engine)
add_library(chess_engine cannon.cpp chariot.cpp chessbase.cpp global.cpp horse.cpp king.cpp minister.cpp official.cpp soldier.cpp)
