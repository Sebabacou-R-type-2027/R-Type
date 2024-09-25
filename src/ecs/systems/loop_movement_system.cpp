/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** loop_movement_system
*/

#include "loop_movement_system.hpp"
#include <chrono>

namespace ecs::systems {

    void LoopMovementSystem::update(Registry& registry) {
        auto &positions = registry.get_components<Position>();
        auto &loop_movements = registry.get_components<LoopMovement>();

        for (std::size_t i = 0; i < positions.size() && i < loop_movements.size(); ++i) {
            if (positions[i] && loop_movements[i]) {
                auto &position = *positions[i];
                auto &loop = *loop_movements[i];
                position.x += loop.speed_x;
                if (position.x > loop.max_x) {
                    if (position.x > loop.max_x) {
                        position.x = loop.min_x;
                } else if (position.x < loop.min_x) {
                    position.y += loop.speed_y;
                }

                position.y += loop.speed_y;
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
    }
}