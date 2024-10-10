/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** loop_movement_system
*/

#include "loop_movement_system.hpp"
#include <chrono>
#include <iostream>
#include <math.h>

namespace ecs::systems {

    void LoopMovementSystem::update(Registry& registry, float deltaTime) {
        auto &positions = registry.get_components<Position>();
        auto &loop_movements = registry.get_components<LoopMovement>();

        for (std::size_t i = 0; i < positions.size() && i < loop_movements.size(); ++i) {
            if (positions[i] && loop_movements[i]) {
                auto &position = *positions[i];
                auto &loop = *loop_movements[i];

                position.y += loop.speed * deltaTime;

                if (position.y < loop.min_y) {
                    position.y = loop.min_y;
                    loop.speed = -loop.speed;
                }
                else if (position.y > loop.max_y) {
                    position.y = loop.max_y;
                    loop.speed = -loop.speed;
                }

            }
        }
    }
}
