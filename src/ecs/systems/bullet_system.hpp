/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** position_system
*/

#pragma once

#include "registry.hpp"

namespace ecs::systems {
    class BulletSystem {
        public:
            void update(Registry& registry);
    };
}
