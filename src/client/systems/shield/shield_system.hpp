/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** shield_system
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
    class ShieldSystem {
        public:
            /**
             * @brief Update the position system
             *
             * @param registry contains all the logic of the components
             */

            void update(ecs::Registry& registry);

        private:
            sf::Time currentTime;
            bool bonus_shield_create = false;
            ecs::Entity shield;
            bool bonus_shield = true;
            sf::Clock timeLimitBonusClock;
            float bonus1_activationTime = 0;
            const float bonusshield_duration = 15.0f;

    };
}
