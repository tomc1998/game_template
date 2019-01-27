#include <entt/entt.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "matrix.hpp"
#include "components.hpp"
#include "shader.hpp"
#include "renderer/renderer.hpp"
#include "setup_context.hpp"

void update(entt::DefaultRegistry &registry) {
  auto view = registry.view<cpos, cvel>();

  for(auto entity: view) {
    // gets only the components that are going to be used ...

    auto &velocity = view.get<cvel>(entity);

    velocity.dx = 0.;
    velocity.dy = 0.;

    // ...
  }
}

void update(std::uint64_t dt, entt::DefaultRegistry &registry) {
  registry.view<cpos, cvel>().each([dt](auto entity, auto &position, auto &velocity) {
      position.x += velocity.dx * dt;
      position.y += velocity.dy * dt;
    });

  // Bounce
  registry.view<cpos, cvel>().each([dt](auto entity, auto &position, auto &velocity) {
      if (position.x > 800.0 || position.x < 0.0) { velocity.dx = -velocity.dx; }
      if (position.y > 600.0 || position.y < 0.0) { velocity.dy = -velocity.dy; }
    });
}

int main() {
  // Create state
  entt::DefaultRegistry registry;
  const std::uint64_t dt = 16;
  unsigned char color[4] {255, 255, 0, 255};
  for(auto ii = 0; ii < 100000; ++ii) {
    auto e = registry.create();
    float x = 800.0 * (float)rand() / (float)RAND_MAX;
    float y = 600.0 * (float)rand() / (float)RAND_MAX;
    float vx = 0.001 * (-1.0 + 2.0 * (float)rand() / (float)RAND_MAX);
    float vy = 0.001 * (-1.0 + 2.0 * (float)rand() / (float)RAND_MAX);
    registry.assign<cpos>(e, x, y);
    registry.assign<cdebug_draw>(e, 0.5f, 0.5f, color);
    registry.assign<cvel>(e, vx, vy);
  }

  // Create window & renderer
  const auto window = setup_context();
  renderer game_renderer(registry);

  glClearColor(0.0, 0.0, 0.0, 1.0);

  while(!glfwWindowShouldClose(window)) {
    update(dt, registry);
    game_renderer.render();
    glfwPollEvents();
    glfwSwapBuffers(window);
  }
}
