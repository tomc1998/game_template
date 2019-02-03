#include <entt/entt.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <lua.hpp>
#include <png.h>

#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>

#include "load_image.hpp"
#include "script_test.hpp"
#include "vector.hpp"
#include "matrix.hpp"
#include "components.hpp"
#include "shader.hpp"
#include "renderer/renderer.hpp"
#include "setup_context.hpp"
#include "grid.hpp"

#include "tests/test_runner.hpp"

void update(GLFWwindow* window, entt::DefaultRegistry &registry) {
  registry.view<cpos, cvel>().each([](auto entity, cpos &pos, cvel &vel) {
      if (pos.vec.x > 800.0 || pos.vec.x < 0.0) { vel.vec.x = -vel.vec.x; }
      if (pos.vec.y > 600.0 || pos.vec.y < 0.0) { vel.vec.y = -vel.vec.y; }
      pos.vec.x += vel.vec.x;
      pos.vec.y += vel.vec.y;
    });
}

int main(int argc, char** argv) {
  if (argc > 1 && strcmp(argv[1], "test") == 0) {
    return tests::run_all_tests();
  }

  // Create state
  entt::DefaultRegistry registry;
  for(auto ii = 0; ii < 4; ++ii) {
    auto e = registry.create();
    unsigned char color[4] {5, 5, (unsigned char)((rand() % 30) + 20), 255};
    float x = 800.0 * (float)rand() / (float)RAND_MAX;
    float y = 600.0 * (float)rand() / (float)RAND_MAX;
    float vx = -1.0 + 2.0 * (float)rand() / (float)RAND_MAX;
    float vy = -1.0 + 2.0 * (float)rand() / (float)RAND_MAX;
    registry.assign<cpos>(e, vec2 { x, y });
    registry.assign<cdebug_draw>(e, vec2 { 30.0f, 30.0f }, color);
    registry.assign<cvel>(e, vec2 { vx, vy });
  }

  // Create window & renderer
  auto window = setup_context();
  renderer game_renderer(registry);

  do_script_test();

  glClearColor(0.0, 0.0, 0.0, 1.0);

  while(!glfwWindowShouldClose(window)) {
    update(window, registry);
    game_renderer.render();
    glfwPollEvents();
    glfwSwapBuffers(window);
  }
}
