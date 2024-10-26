/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** position_system
*/

#pragma once

#include "registry.hpp"
#include "sound_effect.hpp"
#include <SFML/Audio.hpp>

#include "../power-up/powerup_system.hpp"

namespace ecs::systems {
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
            sf::Time currentTime;
            sf::Clock shootClock; ///< Clock of the shoot
            sf::Time lastShootTime = sf::Time::Zero;; ///< Last time the entity shoot
            const sf::Time shootCooldown = sf::seconds(0.5f); ///< Cooldown between two shoots

            sf::Clock clock; ///< Clock of the system
            sf::Time elapsed = sf::Time::Zero; ///< Elapsed time
            int compteur = 0; ///< Compteur
            bool check = false; ///< Check if the bullet is charged
            ecs::SoundEffect Sound_game; ///< Sound of the game

            bool bonus1_activate = false;

            sf::Clock timeLimitBonus1Clock;
            float bonus1_activationTime = 0;
            const float bonus1_duration = 15.0f;

    };
}
