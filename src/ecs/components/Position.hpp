/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Position
*/

#pragma once

namespace ecs {
    /**
     * @brief Class that contains the position of an entity
     */
    struct Position {
        float x, y;

        /**
         * @brief Construct a new Position object
         *
         */
        Position() : x(0), y(0) {}
        /**
         * @brief Construct a new Position object
         * 
         * @param left Position on the x axis
         * @param top Position on the y axis
         */
        Position(float left, float top) : x(left), y(top) {}
    };
}
