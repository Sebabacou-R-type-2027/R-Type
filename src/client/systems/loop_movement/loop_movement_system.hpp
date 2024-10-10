/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** loop_movement_system
*/

#pragma once

#include "registry.hpp"

namespace ecs::systems {
    class LoopMovementSystem {
        public:
            void update(Registry& registry, float deltaTime);
    };
}
