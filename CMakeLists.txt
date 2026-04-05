cmake_minimum_required(VERSION 3.16)
project(ChessEngine)

set(CMAKE_CXX_STANDARD 20)

# Collect sources by module
file(GLOB_RECURSE BOARD_SRC src/board/*.cpp)
file(GLOB_RECURSE MOVE_SRC src/move/*.cpp)
file(GLOB_RECURSE SEARCH_SRC src/search/*.cpp)
file(GLOB_RECURSE EVAL_SRC src/eval/*.cpp)
file(GLOB_RECURSE MAIN_SRC src/main.cpp)

add_executable(chess_engine
    ${MAIN_SRC}
    ${BOARD_SRC}
    ${MOVE_SRC}
    ${SEARCH_SRC}
    ${EVAL_SRC}
)

target_include_directories(chess_engine PRIVATE include)