cmake_minimum_required(VERSION 3.5)

FILE(GLOB_RECURSE ENGINE_SRC src/ *.c)

add_library(engine ${ENGINE_SRC})

target_include_directories(engine PUBLIC "include")
target_link_libraries(engine PUBLIC cglm)

if (NOT EMSCRIPTEN)
  target_link_libraries(engine PUBLIC glfw)
  target_link_libraries(engine PUBLIC glad)
endif()

target_include_directories(engine PRIVATE "../modules/stbi/include")


# include dirs
# target_include_directories(engine PRIVATE "../extern/cglm/include")
# target_include_directories(engine PRIVATE "../extern/freetype/include")

# target_include_directories(engine PRIVATE "../lib/glad/include")

# linking
#target_link_libraries(engine PRIVATE glad)
#target_link_libraries(engine PRIVATE cglm)
#target_link_libraries(engine PRIVATE freetype)
#if (NOT EMSCRIPTEN)
#  target_link_libraries(engine PRIVATE glfw)
#endif()
