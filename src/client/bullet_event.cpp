/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** position_system
*/

#include "bullet_event.hpp"
#include <iostream>


namespace rtype::game {


    void BulletSystem::update(ecs::Registry& registry) {
        auto& positions = registry.get_components<ecs::Position>();
        auto& velocities = registry.get_components<ecs::Velocity>();
        auto& bullet = registry.get_components<ecs::Bullet>();

        sf::Time currentTime = shootClock.getElapsedTime();

        bool check_charge = false;
        static sf::Clock spacePressClock; // Horloge pour mesurer le temps d'appui
        static bool isSpacePressed = false; // État précédent de la touche espace

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {


            if (isSpacePressed == false) {
                    // Si on vient juste d'appuyer sur espace, redémarre l'horloge
                    isSpacePressed = true;
                    spacePressClock.restart();
            }


            if (clock.getElapsedTime().asSeconds() >= 2.0f) {
                // Faire une boucle de deux segonde avec le component chargé image
                //puis executer le reste apres
                clock.restart();
                auto laser_charge = registry.spawn_entity();
                registry.emplace_component<ecs::Velocity>(laser_charge, 45.0f, 0.0f);
                registry.emplace_component<ecs::EntityType>(laser_charge, ecs::Type::Bullet);
                auto &hit = registry.emplace_component<ecs::Hitbox>(laser_charge, ecs::ShapeType::Rectangle, false);
                hit->rect = sf::RectangleShape(sf::Vector2f(20.0f, 20.0f));

                registry.emplace_component<ecs::Position>(laser_charge, positions[0]->x + 40, positions[0]->y + 5);
                registry.emplace_component<ecs::Drawable>(laser_charge, "assets/Bullets/11.png");
                registry.emplace_component<ecs::BulletCharge>(laser_charge);

            }
        } else {
            if (isSpacePressed) {

                // Si on vient de relâcher la touche espace
                isSpacePressed = false;
                clock.restart();

                float timePressed = spacePressClock.getElapsedTime().asSeconds();

                if (timePressed < 1 && currentTime - lastShootTime > shootCooldown) {
                    lastShootTime = currentTime;
                    auto laser_entity = registry.spawn_entity();
                    registry.emplace_component<ecs::Velocity>(laser_entity, 35.0f, 0.0f);
                    registry.emplace_component<ecs::EntityType>(laser_entity, ecs::Type::Bullet);
                    auto &hit = registry.emplace_component<ecs::Hitbox>(laser_entity, ecs::ShapeType::Rectangle, false);
                    hit->rect = sf::RectangleShape(sf::Vector2f(20.0f, 20.0f));
                    registry.emplace_component<ecs::Position>(laser_entity, positions[0]->x + 40, positions[0]->y + 5);
                    registry.emplace_component<ecs::Drawable>(laser_entity, "assets/Bullets/01.png");
                    registry.emplace_component<ecs::Bullet>(laser_entity);
                } else {
                    ChargedOneDraw = true;
                }

                if (timePressed > 1.0f) {
                    ChargedOneDraw = true;
                }
            }
        }
    }
}


