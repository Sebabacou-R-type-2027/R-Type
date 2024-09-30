/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** position_system
*/

#pragma once

#include "registry.hpp"

namespace ecs::systems {
    class BulletSystem {
        public:
            void update(Registry& registry);
            bool ChargedOneDraw = false;

        private:
            sf::Clock shootClock;           // Horloge pour mesurer le temps écoulé
            sf::Time lastShootTime;         // Temps du dernier tir
            const sf::Time shootCooldown = sf::seconds(0.5f);

            sf::Clock clock;
            sf::Time elapsed = sf::Time::Zero; // Temps écoulé total
            int compteur = 0; // Compteur de secondes

    };
}
