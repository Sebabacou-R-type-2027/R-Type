/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Chasing
*/

#pragma once

namespace ecs {
    /**
     * @brief Class that contains the Chasing of an entity
     */
    struct Chasing {
        float speed;

        /**
         * @brief Construct a new Chasing object
         *
         */
        Chasing(float speed = 1.0f)
            : speed(speed) {};
    };
}