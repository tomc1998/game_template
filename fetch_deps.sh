#!/bin/bash

mkdir -p deps

## ENTT
if [ ! -d deps/entt ]; then
  pushd deps && git clone https://github.com/skypjack/entt.git entt
  popd
fi


## GLFW
if [ ! -d deps/glfw ]; then
    pushd deps
    git clone https://github.com/glfw/glfw
    GLFW_CMAKE_FLAGS="-DGLFW_BUILD_DOCS=OFF -DGLFW_BUILD_EXAMPLES=OFF \
                -DGLFW_BUILD_TESTS=OFF -DGLFW_INSTALL=OFF "
    pushd glfw
    mkdir -p build 
    pushd build
    cmake ${GLFW_CMAKE_FLAGS} .. && make
    popd
    popd
    popd
fi

## GLAD
# Just build, source should be in repo
pushd src-deps/glad && gcc -c src/glad.c -ldl -o glad.o -Iinclude
popd

## LIBPNG
if [ ! -d deps/libpng ]; then
    pushd deps
    git clone git://git.code.sf.net/p/libpng/code libpng
    pushd libpng
    mkdir -p build && ./configure --prefix=${PWD}/build && make check && make install
    popd
    popd
fi



