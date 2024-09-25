/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** position_system
*/

#include "bullet_system.hpp"

namespace ecs::systems {

void BulletSystem::update(Registry& registry) {
    auto& positions = registry.get_components<Position>();
    auto& velocities = registry.get_components<Velocity>();
    auto& bullet = registry.get_components<Bullet>();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        auto laser_entity = registry.spawn_entity();
        registry.emplace_component<ecs::Velocity>(laser_entity, 5.0f, 0.0f);
        registry.emplace_component<ecs::Position>(laser_entity, positions[0]->x, positions[0]->y);
        registry.emplace_component<ecs::Drawable>(laser_entity, "assets/Bullets/laser1.png");
    }
}

}
