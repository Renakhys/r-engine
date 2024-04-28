cmake_minimum_required(VERSION 3.5)

FILE(GLOB_RECURSE GAME_SRC "src/*.c")

add_executable(game ${GAME_SRC})

# linking
target_link_libraries(game PRIVATE engine)

target_include_directories(game PRIVATE src)
