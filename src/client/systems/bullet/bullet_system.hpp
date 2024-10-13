/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** position_system
*/

#pragma once

#include "registry.hpp"
#include "Sound_game.hpp"
#include <SFML/Audio.hpp>


namespace rtype::game {
    /**
     * @brief Class that contains all the logic of the position system
     */
    class BulletSystem {
        public:
            /**
             * @brief Update the position system
             *
             * @param registry contains all the logic of the components
             */
            void update(ecs::Registry& registry);
            bool ChargedOneDraw = false;

        private:
            sf::Clock shootClock; ///< Clock of the shoot
            sf::Time lastShootTime; ///< Last time the entity shoot
            const sf::Time shootCooldown = sf::seconds(0.5f); ///< Cooldown between two shoots

            sf::Clock clock; ///< Clock of the system
            sf::Time elapsed = sf::Time::Zero; ///< Elapsed time
            int compteur = 0; ///< Compteur
            bool check = false; ///< Check if the bullet is charged
            rtype::game::Sound_game Sound_game; ///< Sound of the game

    };
}
