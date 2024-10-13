/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** position_system
*/

#pragma once

#include "registry.hpp"

namespace ecs::systems {
    /**
     * @brief Class that contains all the logic of the position system
     */
    class PositionSystem {
        public:
            /**
             * @brief Update the position system
             *
             * @param registry contains all the logic of the components
             */
            void update(Registry& registry);
    };
}
