/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** systems
*/

#include "systems.hpp"


void System::position_system(Registry& registry) {
    auto& positions = registry.get_components<Position>();
    auto& velocities = registry.get_components<Velocity>();

    for (std::size_t i = 0; i < positions.size(); ++i) {
        if (positions[i] && velocities[i]) {
            positions[i]->x += velocities[i]->vx;
            positions[i]->y += velocities[i]->vy;
        }
    }
}

void System::control_system(Registry& registry) {
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

void System::draw_system(Registry& registry, sf::RenderWindow& window, sf::CircleShape& playerShape) {
    auto& positions = registry.get_components<Position>();

    for (std::size_t i = 0; i < positions.size(); ++i) {
        if (positions[i]) {
            playerShape.setPosition(positions[i]->x, positions[i]->y);
            window.draw(playerShape);
        }
    }
}

void System::draw_system(Registry& registry, sf::RenderWindow& window, sf::Sprite& playerSprite) {
    auto& positions = registry.get_components<Position>();

    for (std::size_t i = 0; i < positions.size(); ++i) {
        if (positions[i]) {
            playerSprite.setPosition(positions[i]->x, positions[i]->y);
            window.draw(playerSprite);
        }
    }
}

void System::loop_movement_system(Registry &registry)
{
    auto &positions = registry.get_components<Position>();
    auto &loop_movements = registry.get_components<LoopMovement>();

    for (std::size_t i = 0; i < positions.size() && i < loop_movements.size(); ++i) {
        if (positions[i].has_value() && loop_movements[i].has_value()) {
            auto &position = positions[i].value();
            auto &loop = loop_movements[i].value();

            // position.x += loop.speed * 0.01f;
            if (position.x > loop.max_x) {
                position.x = loop.min_x;
            } else if (position.x < loop.min_x) {
                position.x = loop.max_x;
            }

            position.y += loop.speed * 0.01f;
            if (position.y > loop.max_y) {
                position.y = loop.min_y;
                position.x += 100;
            } else if (position.y < loop.min_y) {
                position.y = loop.max_y;
                position.x += 100;
            }
        }
    }
}