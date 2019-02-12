cmake_minimum_required (VERSION 3.0)
project(entt-game)

set (CMAKE_C_COMPILER gcc)
set (CMAKE_CXX_COMPILER g++)

set (root-dir ${CMAKE_CURRENT_SOURCE_DIR})
set (src-dir ${root-dir}/src)

# Unity build so we just have 1 source file <3
set (entt-game-srcs
  ${src-dir}/main.cpp
  ${root-dir}/src-deps/glad/glad.o)

link_directories(
  ${root-dir}/deps/glfw/build/src
  ${root-dir}/deps/libpng/build/lib
  ${root-dir}/deps/luajit/src
  /usr/local/lib)

# Build the executable

add_executable(entt-game ${entt-game-srcs})

# Setup include directories for the exe
set (entt-game-idir ${root-dir}/src-deps/glad/include
                    ${root-dir}/deps/glfw/include
                    ${root-dir}/deps/entt/src
                    ${root-dir}/deps/libpng/build/include
                    ${root-dir}/deps/luajit/src)
target_include_directories(entt-game PUBLIC ${entt-game-idir})

# run pkg-config for glfw
set(ENV{PKG_CONFIG_PATH} ${root-dir}/deps/glfw/build/src)
execute_process(COMMAND pkg-config --libs-only-l --static glfw3
  COMMAND sed -e s/-l//g
  OUTPUT_VARIABLE glfw-pkg-config)
string(STRIP ${glfw-pkg-config} glfw-pkg-config)
separate_arguments(glfw-pkg-config)

# Link against LLVM libraries
target_link_libraries(entt-game luajit png ${glfw-pkg-config})

# Figure out per-platform compiler flags
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set (CMAKE_CXX_FLAGS "--std=c++14 -Wall -Werror -g -O3 -Wno-pragma-once-outside-header")
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
  set (CMAKE_CXX_FLAGS "-std=c++14 -Wall -Werror -g -O3 -Wno-pragma-once-outside-header")
endif()

