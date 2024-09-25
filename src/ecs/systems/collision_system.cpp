/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** collision_system
*/

#include "collision_system.hpp"
#include "components.hpp"
#include <iostream>

namespace ecs::systems {

bool isColliding(const Position& pos1, const Size& size1, const Position& pos2, const Size& size2) {
    return !(pos1.x + size1.width <= pos2.x ||
            pos1.x >= pos2.x + size2.width ||
            pos1.y + size1.height <= pos2.y ||
            pos1.y >= pos2.y + size2.height);
}

void ecs::systems::CollisionSystem::update(Registry& registry, sf::RenderWindow& window) {
    auto& positions = registry.get_components<Position>();
    auto& sizes = registry.get_components<Size>();

    for (std::size_t i = 0; i < positions.size(); ++i) {
        for (std::size_t j = i + 1; j < positions.size(); ++j) {
            if (positions[i] && sizes[i] && positions[j] && sizes[j] &&
                isColliding(*positions[i], *sizes[i], *positions[j], *sizes[j])) {
                std::cout << "Collision detected between entity " << i << " and entity " << j << std::endl;
            }
        }
    }
}
}