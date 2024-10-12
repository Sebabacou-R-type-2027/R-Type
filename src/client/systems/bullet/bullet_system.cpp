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
        auto &positions = registry.get_components<Position>();
        auto &entityTypes = registry.get_components<EntityType>(); // To identify the player
        auto &draws = registry.get_components<Drawable>();

        sf::Time currentTime = shootClock.getElapsedTime();

        static sf::Clock spacePressClock; // Clock to measure space key press duration
        static bool isSpacePressed = false; // Previous state of the space key

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
                for (size_t i = 0; i < entityTypes.size() && i < positions.size() && i < draws.size(); i++) {
                    if (entityTypes[i].has_value() && positions[i].has_value() && draws[i].has_value()) {
                        if (entityTypes[i]->current_type == Type::Player) {
                            if (draws[i]->sprite.getGlobalBounds().width > 0) {
                                // If space was just released
                                isSpacePressed = false;
                                float timePressed = spacePressClock.getElapsedTime().asSeconds();

                                if (timePressed < 1 && currentTime - lastShootTime > shootCooldown) {
                                    lastShootTime = currentTime;
                                    auto laser_entity = registry.spawn_entity();
                                    registry.emplace_component<ecs::Velocity>(laser_entity, 10.0f, 0.0f);
                                    registry.emplace_component<ecs::EntityType>(laser_entity, ecs::Type::Bullet);
                                    auto &draw = registry.emplace_component<ecs::Drawable>(laser_entity, "assets/Bullets/01.png");
                                    auto& hit = registry.emplace_component<ecs::Hitbox>(laser_entity, ecs::ShapeType::Rectangle, false, true);
                                    hit->rect.setPosition(sf::Vector2f(draws[i]->sprite.getGlobalBounds().width / 2, draws[i]->sprite.getGlobalBounds().height / 2));
                                    hit->rect = sf::RectangleShape(sf::Vector2f(draw->sprite.getGlobalBounds().width / 2, draw->sprite.getGlobalBounds().height / 2));
                                    hit->rect.setOutlineColor(sf::Color::Red);
                                    hit->rect.setOutlineThickness(1.0f);
                                    registry.emplace_component<ecs::Position>(laser_entity,
                                                                                positions[i]->x,
                                                                                positions[i]->y);
                                    registry.emplace_component<ecs::Bullet>(laser_entity);
                                    registry.emplace_component<ecs::CollisionState>(laser_entity, false);
                                    registry.emplace_component<ecs::LifeState>(laser_entity, true);
                                } else if (timePressed > 1.0f) {
                                    ChargedOneDraw = true;
                                }
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

}