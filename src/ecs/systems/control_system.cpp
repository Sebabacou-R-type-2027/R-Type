/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** control_system
*/

#include "control_system.hpp"

namespace ecs::systems {

void ControlSystem::update(Registry& registry) {
    auto& velocities = registry.get_components<Velocity>();
    auto& controllables = registry.get_components<Controllable>();

    for (std::size_t i = 0; i < controllables.size(); ++i) {
        if (controllables[i] && velocities[i]) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                velocities[i]->vx += acceleration;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                velocities[i]->vx -= acceleration;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                velocities[i]->vy -= acceleration;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                velocities[i]->vy += acceleration;
            }

            if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                velocities[i]->vx *= 0.9f;
            }
            if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                velocities[i]->vy *= 0.9f;
            }

            velocities[i]->vx = std::clamp(velocities[i]->vx, -maxSpeed, maxSpeed);
            velocities[i]->vy = std::clamp(velocities[i]->vy, -maxSpeed, maxSpeed);
        }
    }
}

}