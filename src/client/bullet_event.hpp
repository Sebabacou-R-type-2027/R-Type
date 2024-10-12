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


namespace rtype::Game {
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

    };
}
