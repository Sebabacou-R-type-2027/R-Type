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
    class BulletSystem {
        public:
            void update(ecs::Registry& registry);
            bool ChargedOneDraw = false;

        private:
            sf::Clock shootClock;           // Horloge pour mesurer le temps écoulé
            sf::Time lastShootTime;         // Temps du dernier tir
            const sf::Time shootCooldown = sf::seconds(0.5f);

            sf::Clock clock;
            sf::Time elapsed = sf::Time::Zero; // Temps écoulé total
            int compteur = 0; // Compteur de secondes
            bool check = false;
            rtype::game::Sound_game Sound_game;

            bool bonus1_activate = true;

            sf::Clock timeLimitBonus1Clock;
            float bonus1_activationTime = 0;
            const float bonus1_duration = 15.0f;
    };
}
