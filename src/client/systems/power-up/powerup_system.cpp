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

            auto &hit1 = registry.emplace_component<ecs::Hitbox>(powerup1, ecs::ShapeType::Rectangle, false);
            hit1->rect = sf::RectangleShape(sf::Vector2f(20.0f, 20.0f));
            registry.emplace_component<ecs::Position>(powerup1, 500, 600);
            registry.emplace_component<ecs::Drawable>(powerup1, "assets/Power-Up/power1.png");
        }
        // faire un if hitbox is touch mettre le bool  a true
    }
}