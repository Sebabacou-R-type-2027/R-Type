/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** position_system
*/

#include "position_system.hpp"

namespace ecs::systems {

void PositionSystem::update(Registry& registry) {
    auto positions = registry.get_components<Position>();
    auto velocities = registry.get_components<Velocity>();

    for (std::size_t i = 0; i < positions.size(); ++i) {
        if (positions[i] && velocities[i]) {
            positions[i]->get().x += velocities[i]->get().vx;
            positions[i]->get().y += velocities[i]->get().vy;
        }
    }
}

}
