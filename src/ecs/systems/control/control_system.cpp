/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** control_system
*/

#include "control_system.hpp"

namespace ecs::systems {

void ControlSystem::update(Registry& registry) {
    auto &velocities = registry.get_components<Velocity>();
    auto &controllables = registry.get_components<Controllable>();

    for (std::size_t i = 0; i < controllables.size() && i < velocities.size(); ++i) {
        if (controllables[i] && controllables[i]->active && velocities[i]) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                velocities[i]->vy = -controllables[i]->speed;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                velocities[i]->vy = controllables[i]->speed;
            } else {
                velocities[i]->vy = 0;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                velocities[i]->vx = -controllables[i]->speed;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                velocities[i]->vx = controllables[i]->speed;
            } else {
                velocities[i]->vx = 0;
            }

        }
    }
}

}