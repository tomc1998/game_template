cmake_minimum_required (VERSION 3.0)
project(entt-game)

set (CMAKE_C_COMPILER gcc)
set (CMAKE_CXX_COMPILER g++)

set (root-dir ${CMAKE_CURRENT_SOURCE_DIR})
set (src-dir ${root-dir}/src)

set (entt-game-srcs
  ${src-dir}/main.cpp
  )

link_directories(
  ${root-dir}/deps/glfw/build/src
  ${root-dir}/deps/libpng/build/lib
  /usr/local/lib)

add_executable(entt-game ${entt-game-srcs})

target_include_directories(entt-game PUBLIC ${root-dir}/deps/glfw/include)
target_include_directories(entt-game PUBLIC ${root-dir}/deps/entt/src)
target_include_directories(entt-game PUBLIC ${root-dir}/deps/libpng/build/include)

# run pkg-config for glfw
set(ENV{PKG_CONFIG_PATH} ${root-dir}/deps/glfw/build/src)
execute_process(COMMAND pkg-config --libs-only-l --static glfw3
  COMMAND sed -e s/-l//g
  OUTPUT_VARIABLE glfw-pkg-config)
string(STRIP ${glfw-pkg-config} glfw-pkg-config)
separate_arguments(glfw-pkg-config)

# Link against LLVM libraries
target_link_libraries(entt-game png ${glfw-pkg-config})

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set (CMAKE_CXX_FLAGS "--std=c++14 -Wall -Werror -g -O3")
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
  set (CMAKE_CXX_FLAGS "-std=c++14 -Wall -Werror -g -O3")
endif()

