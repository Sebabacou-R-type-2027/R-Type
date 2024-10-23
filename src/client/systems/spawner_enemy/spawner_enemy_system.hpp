/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Spawner_enemy_system
*/

#pragma once

#include "registry.hpp"
#include "create_enemies.hpp"
#include <SFML/System/Clock.hpp>

namespace ecs::systems {

    /**
     * @brief Class that contains all the logic of the shoot enemy system
     * 
     */
    class SpawnerEnemySystem {
        public:
            /**
             * @brief Update the shoot enemy system
             * 
             * @param registry contains all the logic of the components
             * @param window contains the window of the game
             */
            void update(Registry& registry, sf::RenderWindow& window);
        private:
            CreateEnemies _createEnemies;
    };
}