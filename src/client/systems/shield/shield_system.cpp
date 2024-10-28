/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** shield_system
*/


#include "shield_system.hpp"
#include "sound_effect.hpp"
#include <SFML/Audio.hpp>
#include "utils/CheckEntity.hpp"

namespace ecs::systems {

    void ShieldSystem::update(ecs::Registry& registry) {
        auto& positions = registry.get_components<ecs::Position>();
        auto& shield = registry.get_components<ecs::Shield>();
        auto& lifestate = registry.get_components<ecs::LifeState>();


        if (bonus_shield == true) {
            if (currentTime.asSeconds() - bonus1_activationTime > bonusshield_duration) {
                bonus_shield = false;
                bonus_shield_create = false;
            }
        }
            if (bonus_shield == true) {
                if (!bonus_shield_create) {
                    auto shield = registry.spawn_entity();
                    bonus_shield_create = true;

                    auto &draw1 = registry.emplace_component<ecs::Drawable>(shield, "assets/Power-Up/shield.png");
                    registry.emplace_component<ecs::EntityType>(shield, ecs::Type::Shield);

                    auto &hit1 = registry.emplace_component<ecs::Hitbox>(shield, ecs::ShapeType::Rectangle, false);
                    hit1->rect.setPosition(100, positions[CheckEntity(registry, ecs::Type::Shield)]->y + 5);
                    hit1->rect = sf::RectangleShape(sf::Vector2f(draw1->sprite.getGlobalBounds().width, draw1->sprite.getGlobalBounds().height));

                    registry.emplace_component<ecs::Position>(
                        shield,
                        positions[CheckEntity(registry, ecs::Type::Player)]->x + 85,
                        positions[CheckEntity(registry, ecs::Type::Player)]->y + 5
                    );

                    registry.emplace_component<ecs::Shield>(shield);
                    registry.emplace_component<ecs::LifeState>(shield, true);
                    registry.emplace_component<ecs::CollisionState>(shield, false);
                }

            }
    }
}
