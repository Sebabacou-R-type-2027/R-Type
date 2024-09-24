/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Controllable
*/

#pragma once

namespace ecs {
    struct Controllable {
        bool active;
        float speed;

        Controllable(bool active = true, float speed = 100.0f)
            : active(active), speed(speed) {}
    };
}
