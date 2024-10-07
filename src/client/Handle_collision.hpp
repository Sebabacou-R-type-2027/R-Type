/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Handle_collision
*/

#include <iostream>
#include "registry.hpp"

namespace ecs {
    class HandleCollision {
        public:
            HandleCollision() = default;
            ~HandleCollision() = default;
            void handle_collision(Registry& registry);
    };
}
