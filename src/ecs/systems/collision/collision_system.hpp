/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** collision_system
*/

#pragma once

#include "registry.hpp"
#include <SFML/Graphics.hpp>

namespace ecs::systems {

    /**
     * @brief Class that contains all the logic of the collision system
     */
    class CollisionSystem {
        public:
            /**
             * @brief Update the collision system
             *
             * @param registry contains all the logic of the components
             * @param window contains the window of the game
             */
            void update(Registry& registry, sf::RenderWindow& window);
    };

}
