/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Collision_state
*/

#pragma once

namespace ecs {
    struct CollisionState {
        bool active;

        CollisionState (bool active = false)
            : active(active) {}
    };
}
