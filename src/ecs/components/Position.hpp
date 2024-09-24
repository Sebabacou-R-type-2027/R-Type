/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Position
*/

#pragma once

namespace ecs {
    struct Position {
        float x = 0;
        float y = 0;
        Position(float x = 0, float y = 0) : x(x), y(y) {}
    };
}
