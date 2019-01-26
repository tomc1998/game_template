mkdir -p build
cd build && cmake .. && make VERBOSE=1 -j 3 && cd .. && build/entt-game
