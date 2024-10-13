/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Position
*/

#pragma once

namespace ecs {
    struct Position {
        float x, y;

        Position() : x(0), y(0) {}
        Position(float left, float top) : x(left), y(top) {}
    };
}
