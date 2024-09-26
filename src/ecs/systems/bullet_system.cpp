/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** position_system
*/

#include "bullet_system.hpp"
#include <iostream>

namespace ecs::systems {


    void BulletSystem::update(Registry& registry) {
        auto& positions = registry.get_components<Position>();
        auto& velocities = registry.get_components<Velocity>();
        auto& bullet = registry.get_components<Bullet>();

        sf::Time currentTime = shootClock.getElapsedTime();


        static sf::Clock spacePressClock; // Horloge pour mesurer le temps d'appui
        static bool isSpacePressed = false; // État précédent de la touche espace


        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {


        if (isSpacePressed == false) {
                // Si on vient juste d'appuyer sur espace, redémarre l'horloge
                isSpacePressed = true;
                spacePressClock.restart();
            }

        } else {
            if (isSpacePressed) {
                // Si on vient de relâcher la touche espace
                isSpacePressed = false;
                float timePressed = spacePressClock.getElapsedTime().asSeconds();

                if (timePressed < 1 && currentTime - lastShootTime > shootCooldown) {
                    lastShootTime = currentTime;
                    auto laser_entity = registry.spawn_entity();
                    registry.emplace_component<ecs::Velocity>(laser_entity, 35.0f, 0.0f);
                    registry.emplace_component<ecs::Position>(laser_entity, positions[0]->x, positions[0]->y);
                    registry.emplace_component<ecs::Drawable>(laser_entity, "assets/Bullets/01.png");
                }

                if (timePressed > 1.5 && timePressed < 3) {

                }
            }
        }
    }
}
