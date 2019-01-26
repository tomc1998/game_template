#include <entt/entt.hpp>
#include <cstdint>
#include <iostream>

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
    std::uint64_t dt = 16;

    unsigned remembered_entity;
    for(auto ii = 0; ii < 10000; ++ii) {
        remembered_entity = registry.create();
        registry.assign<Position>(remembered_entity, ii * 1.f, ii * 1.f);
        if(ii % 2 == 0) { registry.assign<Velocity>(remembered_entity, ii * .1f, ii * .1f); }
    }

    for (auto ii = 0; ii < 100; ++ii) {
      if (ii % 10 == 0) {
      }
      update(dt, registry);
    }

    std::cout << "Entity = " << remembered_entity << std::endl;

    const auto &pos = registry.get<Position>(remembered_entity - 1);
    std::cout << "Pos = " << pos.x << " " << pos.y << std::endl;
}
