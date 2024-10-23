/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** position_system
*/

#include "powerup_system.hpp"
#include <iostream>
#include "Sound_game.hpp"
#include <SFML/Audio.hpp>
#include "components/Entity_type.hpp"
#include "utils/CheckEntity.hpp"

namespace rtype::game {


    void  PowerUp::update(ecs::Registry& registry) {

        if (isCreate == false) {
            auto powerup1 = registry.spawn_entity();
            isCreate = true;
            auto &hit1 = registry.emplace_component<ecs::Hitbox>(powerup1, ecs::ShapeType::Rectangle, false);
            hit1->rect = sf::RectangleShape(sf::Vector2f(40.0f, 40.0f));

            registry.emplace_component<ecs::Position>(powerup1, 500, 600);
            registry.emplace_component<ecs::EntityType>(powerup1, ecs::Type::PowerUp);
            registry.emplace_component<ecs::Drawable>(powerup1, "assets/Power-Up/power1.png");
            registry.emplace_component<ecs::LifeState>(powerup1, true);
            registry.emplace_component<ecs::CollisionState>(powerup1, false);
        }
        // faire un if hitbox is touch mettre le bool  a true
    }
}