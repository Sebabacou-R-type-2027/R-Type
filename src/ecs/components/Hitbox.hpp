/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Hitbox
*/

#pragma once

#include <SFML/Graphics.hpp>

namespace ecs {
    struct Hitbox {
        float radius = 0;
        bool is_colliding = false;
        sf::RectangleShape rect;

        Hitbox(float radius = 0, bool is_colliding = false, sf::RectangleShape rect = sf::RectangleShape())
            : radius(radius), is_colliding(is_colliding), rect(rect) {}
    };
}

