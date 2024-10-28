/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** position_system
*/

#include "bullet_system.hpp"
#include "sound_effect.hpp"
#include <SFML/Audio.hpp>
#include "components/Entity_type.hpp"
#include "utils/CheckEntity.hpp"

namespace ecs::systems {

    void BulletSystem::update(ecs::Registry& registry) {
        auto& positions = registry.get_components<ecs::Position>();
        auto& velocities = registry.get_components<ecs::Velocity>();
        auto& bullet = registry.get_components<ecs::Bullet>();
        auto& lifestate = registry.get_components<ecs::LifeState>();

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

        } else {
            if (isSpacePressed) {

                // Si on vient de relâcher la touche espace
                isSpacePressed = false;
                clock.restart();

                float timePressed = spacePressClock.getElapsedTime().asSeconds();


                if (timePressed > 1) {
                    sf::Time waitCharge;
                    if (check == false) {
                        clock.restart();
                        if (lifestate[CheckEntity(registry, ecs::Type::Player)]->isAlive == false) {
                            return;
                        }
                        auto laser_charge = registry.spawn_entity();
                        Sound_game.playSound("assets/Son/laser_gun1.wav", 40);
                        registry.emplace_component<ecs::Velocity>(laser_charge, 45.0f, 0.0f);
                        registry.emplace_component<ecs::EntityType>(laser_charge, ecs::Type::ChargedBullet);

                        auto& draw = registry.emplace_component<ecs::Drawable>(laser_charge, "assets/Bullets/11.png");
                        auto &hit = registry.emplace_component<ecs::Hitbox>(laser_charge, ecs::ShapeType::Rectangle, false);
                        hit->rect = sf::RectangleShape(sf::Vector2f(draw->sprite.getGlobalBounds().width, draw->sprite.getGlobalBounds().height));
                        hit->rect.setOutlineColor(sf::Color::Red);
                        hit->rect.setOutlineThickness(1.0f);
                        registry.emplace_component<ecs::Position>(laser_charge, positions[CheckEntity(registry, ecs::Type::Player)]->x + 75, positions[CheckEntity(registry, ecs::Type::Player)]->y - 10); // Use player position

                        registry.emplace_component<ecs::BulletCharge>(laser_charge);
                        registry.emplace_component<ecs::LifeState>(laser_charge, true);
                        registry.emplace_component<ecs::CollisionState>(laser_charge, false);
                        }
                    }


                if (bonus1_activate) {
                    if (currentTime.asSeconds() - bonus1_activationTime > bonus1_duration) {
                        bonus1_activate = false;
                    }
                }
                if (bonus_heal) {
                    if (currentTime.asSeconds() - bonus1_activationTime > bonus1_duration) {
                        bonus_heal = false;
                    }
                }
                if (timePressed < 1 && currentTime - lastShootTime > shootCooldown && bonus_heal == true) {
                    //POWERUP HEAL SHOOT
                    Sound_game.playSound("assets/Son/laser_gun2.wav", 40);
                    lastShootTime = currentTime;

                    auto laser_heal = registry.spawn_entity();

                    auto &draw1 = registry.emplace_component<ecs::Drawable>(laser_heal, "assets/Bullets/08.png");
                    registry.emplace_component<ecs::Velocity>(laser_heal, 35.0f, 0.0f);
                    registry.emplace_component<ecs::EntityType>(laser_heal, ecs::Type::BulletHeal);
                    auto &hit1 = registry.emplace_component<ecs::Hitbox>(laser_heal, ecs::ShapeType::Rectangle, false);
                    hit1->rect.setPosition(100, positions[CheckEntity(registry, ecs::Type::Player)]->y + 5);
                    hit1->rect = sf::RectangleShape(sf::Vector2f(draw1->sprite.getGlobalBounds().width, draw1->sprite.getGlobalBounds().height));
                    registry.emplace_component<ecs::Position>(laser_heal, positions[CheckEntity(registry, ecs::Type::Player)]->x + 75, positions[CheckEntity(registry, ecs::Type::Player)]->y + 5);
                    registry.emplace_component<ecs::BulletHeal>(laser_heal);
                    registry.emplace_component<ecs::LifeState>(laser_heal, true);
                    registry.emplace_component<ecs::CollisionState>(laser_heal, false);

                } else {
                    ChargedOneDraw = true;
                }



                if (timePressed < 1 && currentTime - lastShootTime > shootCooldown && bonus1_activate == true) {
                    //POWERUP 1 THREE SHOOT
                    Sound_game.playSound("assets/Son/laser_gun2.wav", 40);
                    lastShootTime = currentTime;

                    auto laser_entity1 = registry.spawn_entity();
                    auto laser_entity2 = registry.spawn_entity();
                    auto laser_entity3 = registry.spawn_entity();

                    auto &draw1 = registry.emplace_component<ecs::Drawable>(laser_entity1, "assets/Bullets/01.png");
                    registry.emplace_component<ecs::Velocity>(laser_entity1, 35.0f, 0.0f);
                    registry.emplace_component<ecs::EntityType>(laser_entity1, ecs::Type::Bullet);
                    auto &hit1 = registry.emplace_component<ecs::Hitbox>(laser_entity1, ecs::ShapeType::Rectangle, false);
                    hit1->rect.setPosition(100, positions[CheckEntity(registry, ecs::Type::Player)]->y + 5);
                    hit1->rect = sf::RectangleShape(sf::Vector2f(draw1->sprite.getGlobalBounds().width, draw1->sprite.getGlobalBounds().height));
                    registry.emplace_component<ecs::Position>(laser_entity1, positions[CheckEntity(registry, ecs::Type::Player)]->x + 75, positions[CheckEntity(registry, ecs::Type::Player)]->y - 25);
                    registry.emplace_component<ecs::Bullet>(laser_entity1);
                    registry.emplace_component<ecs::LifeState>(laser_entity1, true);
                    registry.emplace_component<ecs::CollisionState>(laser_entity1, false);


                    auto &draw2 = registry.emplace_component<ecs::Drawable>(laser_entity2, "assets/Bullets/01.png");
                    registry.emplace_component<ecs::Velocity>(laser_entity2, 35.0f, 0.0f);
                    registry.emplace_component<ecs::EntityType>(laser_entity2, ecs::Type::Bullet);
                    auto &hit2 = registry.emplace_component<ecs::Hitbox>(laser_entity2, ecs::ShapeType::Rectangle, false);
                    hit2->rect.setPosition(100, positions[CheckEntity(registry, ecs::Type::Player)]->y + 5);
                    hit2->rect = sf::RectangleShape(sf::Vector2f(draw2->sprite.getGlobalBounds().width, draw2->sprite.getGlobalBounds().height));
                    registry.emplace_component<ecs::Position>(laser_entity2, positions[CheckEntity(registry, ecs::Type::Player)]->x + 75, positions[CheckEntity(registry, ecs::Type::Player)]->y + 5);
                    registry.emplace_component<ecs::Bullet>(laser_entity2);
                    registry.emplace_component<ecs::LifeState>(laser_entity2, true);
                    registry.emplace_component<ecs::CollisionState>(laser_entity2, false);


                    auto &draw3 = registry.emplace_component<ecs::Drawable>(laser_entity3, "assets/Bullets/01.png");
                    registry.emplace_component<ecs::Velocity>(laser_entity3, 35.0f, 0.0f);
                    registry.emplace_component<ecs::EntityType>(laser_entity3, ecs::Type::Bullet);
                    auto &hit3 = registry.emplace_component<ecs::Hitbox>(laser_entity3, ecs::ShapeType::Rectangle, false);
                    hit3->rect.setPosition(100, positions[CheckEntity(registry, ecs::Type::Player)]->y + 5);
                    hit3->rect = sf::RectangleShape(sf::Vector2f(draw3->sprite.getGlobalBounds().width, draw3->sprite.getGlobalBounds().height));
                    registry.emplace_component<ecs::Position>(laser_entity3, positions[CheckEntity(registry, ecs::Type::Player)]->x + 75, positions[CheckEntity(registry, ecs::Type::Player)]->y + 40);
                    registry.emplace_component<ecs::Bullet>(laser_entity3);
                    registry.emplace_component<ecs::LifeState>(laser_entity3, true);
                    registry.emplace_component<ecs::CollisionState>(laser_entity3, false);

                } else {
                    ChargedOneDraw = true;
                }

                // TIR CLASSIQUE
                if (timePressed < 1 && currentTime - lastShootTime > shootCooldown && bonus1_activate == false && bonus_heal == false) {
                    if (lifestate[CheckEntity(registry, ecs::Type::Player)]->isAlive == false) {
                        return;
                    }
                    Sound_game.playSound("assets/Son/laser_gun2.wav", 40);
                    lastShootTime = currentTime;
                    auto laser_entity = registry.spawn_entity();
                    registry.emplace_component<ecs::Velocity>(laser_entity, 35.0f, 0.0f);
                    registry.emplace_component<ecs::EntityType>(laser_entity, ecs::Type::Bullet);

                    auto& draw = registry.emplace_component<ecs::Drawable>(laser_entity, "assets/Bullets/01.png");
                    auto &hit = registry.emplace_component<ecs::Hitbox>(laser_entity, ecs::ShapeType::Rectangle, false);
                    // hit->rect.setPosition(100, positions[CheckEntity(registry, ecs::Type::Player)]->y + 5);
                    hit->rect.setPosition(positions[CheckEntity(registry, ecs::Type::Player)]->x + 70, positions[CheckEntity(registry, ecs::Type::Player)]->y + 15);
                    hit->rect = sf::RectangleShape(sf::Vector2f(draw->sprite.getGlobalBounds().width, draw->sprite.getGlobalBounds().height));
                    hit->rect.setOutlineColor(sf::Color::Red);
                    hit->rect.setOutlineThickness(1.0f);

                    registry.emplace_component<ecs::Position>(laser_entity, positions[CheckEntity(registry, ecs::Type::Player)]->x + 70, positions[CheckEntity(registry, ecs::Type::Player)]->y + 15); // Use player position
                    registry.emplace_component<ecs::Bullet>(laser_entity);
                    registry.emplace_component<ecs::LifeState>(laser_entity, true);
                    registry.emplace_component<ecs::CollisionState>(laser_entity, false);
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
