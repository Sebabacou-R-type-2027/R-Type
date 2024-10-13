/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Handle_collision
*/

#include <iostream>
#include "registry.hpp"

namespace ecs {
    /**
     * @brief Class that contains all the logic of the collision system
     */
    class HandleCollision {
        public:
            HandleCollision() = default;
            ~HandleCollision() = default;
            /**
             * @brief Update the collision system
             * 
             * @param registry contains all the logic of the components
             */
            void handle_collision(Registry& registry);
            /**
             * @brief Create explosion
             * 
             * @param registry contains all the logic of the components
             * @param x Position on the x axis
             * @param y Position on the y axis
             */
            void create_explosion(Registry& registry, float x, float y);
    };
}
