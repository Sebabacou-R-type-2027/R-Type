/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** shoot_enemy_system
*/

#pragma once

#include "registry.hpp"
#include <SFML/System/Clock.hpp>

namespace ecs::systems {

    /**
     * @brief Class that contains all the logic of the shoot enemy system
     * 
     */
    class ShootingEnemySystem {
        public:
            /**
             * @brief Update the shoot enemy system
             * 
             * @param registry contains all the logic of the components
             * @param window contains the window of the game
             */
            void update(Registry& registry, sf::RenderWindow& window);
        private:
            sf::Clock clock; ///< Clock of the system
    };
}