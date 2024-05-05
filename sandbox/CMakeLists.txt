cmake_minimum_required(VERSION 3.5)

FILE(GLOB_RECURSE SANDBOX_SRC "src/*.c")

add_executable(sandbox ${SANDBOX_SRC})
target_compile_options(sandbox PRIVATE -Wextra -Wpedantic -Werror)

# linking
target_link_libraries(sandbox PRIVATE engine)

target_include_directories(sandbox PRIVATE src)
