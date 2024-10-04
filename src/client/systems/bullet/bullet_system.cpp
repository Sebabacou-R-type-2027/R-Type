/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** position_system
*/

#include "bullet_system.hpp"
#include <iostream>

namespace ecs::systems {


void BulletSystem::update(Registry& registry) {
    auto& positions = registry.get_components<Position>();
    auto& velocities = registry.get_components<Velocity>();
    auto& bullet = registry.get_components<Bullet>();
    auto& entityTypes = registry.get_components<EntityType>(); // To identify the player

    sf::Time currentTime = shootClock.getElapsedTime();

    bool check_charge = false;
    static sf::Clock spacePressClock; // Clock to measure space key press duration
    static bool isSpacePressed = false; // Previous state of the space key

    // Find the player entity
    std::size_t playerIndex = -1;
    for (std::size_t i = 0; i < entityTypes.size(); ++i) {
        if (entityTypes[i] && entityTypes[i]->current_type == ecs::Type::Player) {
            playerIndex = i;
            break;
        }
    }

    // If no player is found, return early
    if (playerIndex == -1) {
        std::cerr << "Player entity not found!\n";
        return;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        if (isSpacePressed == false) {
            // If space was just pressed, restart the clock
            isSpacePressed = true;
            spacePressClock.restart();
        }

        if (clock.getElapsedTime().asSeconds() >= 1.0f) {
            // If space has been held for more than one second
        }
    } else {
        if (isSpacePressed) {
            // If space was just released
            isSpacePressed = false;
            float timePressed = spacePressClock.getElapsedTime().asSeconds();

            if (timePressed < 1 && currentTime - lastShootTime > shootCooldown) {
                lastShootTime = currentTime;
                auto laser_entity = registry.spawn_entity();
                registry.emplace_component<ecs::Velocity>(laser_entity, 35.0f, 0.0f);
                registry.emplace_component<ecs::EntityType>(laser_entity, ecs::Type::Bullet);
                auto& hit = registry.emplace_component<ecs::Hitbox>(laser_entity, ecs::ShapeType::Rectangle, false);
                hit->rect = sf::RectangleShape(sf::Vector2f(20.0f, 20.0f));
                registry.emplace_component<ecs::Position>(laser_entity, positions[playerIndex]->x + 40, positions[playerIndex]->y + 5); // Use player position
                registry.emplace_component<ecs::Drawable>(laser_entity, "assets/Bullets/01.png");
                registry.emplace_component<ecs::Bullet>(laser_entity);
            } else {
                ChargedOneDraw = true;
            }

            if (timePressed > 1.0f) {
                ChargedOneDraw = true;
            }
        }
    }
}

}