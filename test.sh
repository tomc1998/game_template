mkdir -p build
cd build && cmake .. && make VERBOSE=1 -j 3 && cd .. && time ${VALGRIND} build/entt-game test
