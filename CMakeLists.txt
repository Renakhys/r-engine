cmake_minimum_required(VERSION 3.8)

project(opengl)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")

# engine library
add_subdirectory(engine)
# game 
add_subdirectory(game)

# dependencies
set(CGLM_SHARED ON "Shared build")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(modules/cglm)

if(NOT EMSCRIPTEN)
  add_subdirectory(modules/glad)
  add_subdirectory(modules/glfw)
endif()

if (EMSCRIPTEN)
    #set_target_properties(game PROPERTIES CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} --bind --memory-init-file 0 -s TOTAL_MEMORY=64MB")
    ## Embed files at build-dir/data
    #set_target_properties(game PROPERTIES CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --embed-file \"${CMAKE_SOURCE_DIR}/bin-run\"") #@data
    #
    ## Use webgl2 and glfw3
    #set_target_properties(game PROPERTIES CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s USE_WEBGL2=1 -s USE_GLFW=3")
    #
    #set_target_properties(game PROPERTIES CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s BINARYEN_ASYNC_COMPILATION=1 -s WASM=1 -s BINARYEN=1 -s \"BINARYEN_METHOD='native-wasm'\" -s DISABLE_DEPRECATED_FIND_EVENT_TARGET_BEHAVIOR=1 -o app_wasm2.html")
    ## Enable js output (no html).
    ##set_target_properties(game PROPERTIES CMAKE_EXECUTABLE_SUFFIX "_wasm.js")
    #set_target_properties(game PROPERTIES CMAKE_EXECUTABLE_SUFFIX "_emsdk.html")

  set_target_properties(game PROPERTIES CMAKE_EXECUTABLE_SUFFIX ".html")
  set_target_properties(game PROPERTIES SUFFIX ".html")
	set_target_properties(game PROPERTIES COMPILE_FLAGS "-Os -s SIDE_MODULE=1 ")
	set_target_properties(game PROPERTIES LINK_FLAGS    "-Os -s WASM=0 -s SIDE_MODULE=1 -s STANDALONE_WASM --no-entry")  
  set_target_properties(game PROPERTIES LINK_FLAGS " --bind -s MIN_WEBGL_VERSION=2 -s -g2 -s USE_GLFW=3 --preload-file \"${CMAKE_SOURCE_DIR}/bin-run/assets@assets\"")
endif()