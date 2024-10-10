/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Hitbox
*/

#pragma once

#include <SFML/Graphics.hpp>

namespace ecs {
    enum class ShapeType {
            Rectangle,
            Circle
        };

    struct Hitbox {
        ShapeType shape;
        sf::RectangleShape rect;
        sf::CircleShape circle;
        bool is_colliding;

        Hitbox(ShapeType shape = ShapeType::Rectangle, bool is_colliding = false)
            : shape(shape), is_colliding(is_colliding) {}
    };

}
