/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** control_system
*/

#include "control_system.hpp"

namespace ecs::systems {

void ControlSystem::update(Registry& registry) {
    auto velocities = registry.get_components<Velocity>();
    auto controllables = registry.get_components<Controllable>();

    for (std::size_t i = 0; i < controllables.size(); ++i) {
        if (controllables[i] && velocities[i]) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                velocities[i]->get().vx += acceleration;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                velocities[i]->get().vx -= acceleration;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                velocities[i]->get().vy -= acceleration;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                velocities[i]->get().vy += acceleration;
            }

            if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                velocities[i]->get().vx *= 0.9f;
            }
            if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                velocities[i]->get().vy *= 0.9f;
            }

            velocities[i]->get().vx = std::clamp(velocities[i]->get().vx, -maxSpeed, maxSpeed);
            velocities[i]->get().vy = std::clamp(velocities[i]->get().vy, -maxSpeed, maxSpeed);
        }
    }
}

}