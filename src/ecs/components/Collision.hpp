/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Collision
*/

#pragma once

#include <SFML/Graphics.hpp>

namespace ecs {
    struct Collision {
        float radius = 0;
        bool is_colliding = false;
        sf::Rect<float> rect;

        Collision(float radius = 0, bool is_colliding = false, sf::Rect<float> rect = sf::Rect<float>())
            : radius(radius), is_colliding(is_colliding), rect(rect) {}
    };
}

