/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** animation_system
*/

#include "animation_system.hpp"
#include <chrono>
#include <iostream>

namespace ecs::systems {

    void AnimationSystem::update(Registry& registry, float deltaTime, sf::RenderWindow& window) {
        auto &animations = registry.get_components<ecs::Animation>();
        auto &positions = registry.get_components<ecs::Position>();

        for (std::size_t i = 0; i < animations.size() && positions.size(); i++) {
            if (animations[i] && positions[i]) {
                Animation &animation = *animations[i];
                Position &position = *positions[i];
                animation.sprite.setPosition(position.x, position.y);
                animation.update(deltaTime);
            }
        }
    }

}
