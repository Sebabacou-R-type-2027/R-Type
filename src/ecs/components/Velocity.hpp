/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Velocity
*/

#pragma once

namespace ecs {
    struct Velocity {
        float vx = 0;
        float vy = 0;
        Velocity(float vx = 0, float vy = 0) : vx(vx), vy(vy) {}
    };
}
