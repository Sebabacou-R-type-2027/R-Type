/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** LoopMovement
*/

#pragma once

namespace ecs {
    struct LoopMovement {
        float min_x, max_x;
        float min_y, max_y;
        float speed;
        float angle;
        float radius;
        float angular_speed;

        LoopMovement(float min_x = 0.0f, float max_x = 0.0f, float min_y = 0.0f, float max_y = 0.0f,
                     float speed = 0.0f, float angle = 0.0f, float radius = 0.0f, float angular_speed = 0.0f)
            : min_x(min_x), max_x(max_x), min_y(min_y), max_y(max_y),
            speed(speed), angle(angle), radius(radius), angular_speed(angular_speed) {}
    };
}
