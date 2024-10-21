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
                Position &position = *positions[i];
                LoopMovement &loop = *loop_movements[i];

                // Horizontal movement (x-axis)
                position.x += loop.speed * deltaTime;

                // Check if we need to apply sinusoidal movement (i.e., radius and angular_speed are non-zero)
                if (loop.radius != 0.0f && loop.angular_speed != 0.0f) {
                    // Update the angle based on angular speed
                    loop.angle += loop.angular_speed * deltaTime;

                    // Sinusoidal movement: modify the y-position based on a sine wave
                    position.y = loop.min_y + loop.radius * sin(loop.angle);
                } else {
                    // Simple up-and-down movement (bouncing between min_y and max_y)
                    position.y += loop.speed * deltaTime;

                    // Reverse direction when reaching boundaries
                    if (position.y < loop.min_y) {
                        position.y = loop.min_y;
                        loop.speed = fabs(loop.speed); // Move down
                    } else if (position.y > loop.max_y) {
                        position.y = loop.max_y;
                        loop.speed = -fabs(loop.speed); // Move up
                    }
                }

                // Handle horizontal looping (wrap around)
                if (position.x < loop.min_x) {
                    position.x = loop.max_x;  // Loop from left to right
                }
                if (position.x > loop.max_x) {
                    position.x = loop.min_x;  // Loop from right to left
                }
            }
        }
    }
}
