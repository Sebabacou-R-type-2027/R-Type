/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Hitbox
*/

#pragma once

#include <SFML/Graphics.hpp>

namespace ecs {
    /**
     * @brief Enum to define the shape of the hitbox
     */
    enum class ShapeType {
            Rectangle,
            Circle
        };

    /**
     * @brief Class that contains the hitbox of an entity
     */
    struct Hitbox {
        ShapeType shape;
        sf::RectangleShape rect;
        sf::CircleShape circle;
        bool is_colliding;
        bool debugdisplay;

        /**
         * @brief Construct a new Hitbox object
         *
         * @param shape Shape of the hitbox
         * @param is_colliding If the hitbox is colliding
         * @param debug If the hitbox is displayed
         */
        Hitbox(ShapeType shape = ShapeType::Rectangle, bool is_colliding = false, bool debug = false)
            : shape(shape), is_colliding(is_colliding), debugdisplay(debug){}

    };

}
