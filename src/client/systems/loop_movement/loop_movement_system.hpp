/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** loop_movement_system
*/

#pragma once

#include "registry.hpp"

namespace ecs::systems {
    /**
     * @brief Class that contains all the logic of the loop movement system
     */
    class LoopMovementSystem {
        public:
            /**
             * @brief Update the loop movement system
             *
             * @param registry contains all the logic of the components
             * @param deltaTime contains the time between two frames
             */
            void update(Registry& registry, float deltaTime);
    };
}
