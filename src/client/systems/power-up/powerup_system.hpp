/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** position_system
*/

#pragma once

#include "registry.hpp"


namespace rtype::game {
    class PowerUp {
        public:
            void update(ecs::Registry& registry);
        private:
            bool isCreate = false;

    };
}
