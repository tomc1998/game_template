#include <entt/entt.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <iostream>

#include "shader.hpp"
#include "renderer/renderer.hpp"
#include "setup_context.hpp"

struct Position {
    float x;
    float y;
};

struct Velocity {
    float dx;
    float dy;
};

void update(entt::DefaultRegistry &registry) {
    auto view = registry.view<Position, Velocity>();

    for(auto entity: view) {
        // gets only the components that are going to be used ...

        auto &velocity = view.get<Velocity>(entity);

        velocity.dx = 0.;
        velocity.dy = 0.;

        // ...
    }
}

void update(std::uint64_t dt, entt::DefaultRegistry &registry) {
    registry.view<Position, Velocity>().each([dt](auto entity, auto &position, auto &velocity) {
        position.x += velocity.dx * dt;
        position.y += velocity.dy * dt;
    });
}

int main() {
    entt::DefaultRegistry registry;
    const std::uint64_t dt = 16;
    const auto window = setup_context();
    renderer game_renderer(registry);

    unsigned remembered_entity;
    for(auto ii = 0; ii < 10000; ++ii) {
        remembered_entity = registry.create();
        registry.assign<Position>(remembered_entity, ii * 1.f, ii * 1.f);
        if(ii % 2 == 0) { registry.assign<Velocity>(remembered_entity, ii * .1f, ii * .1f); }
    }

    glClearColor(0.0, 0.0, 0.0, 1.0);

    while(!glfwWindowShouldClose(window)) {
      update(dt, registry);
      game_renderer.render();
      glfwPollEvents();
      glfwSwapBuffers(window);
    }
}
