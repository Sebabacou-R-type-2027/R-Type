/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Velocity
*/

#pragma once

namespace ecs {
    /**
     * @brief Class that contains the velocity of an entity
     */
    struct Velocity {
        float vx = 0;
        float vy = 0;
        /**
         * @brief Construct a new Velocity object
         *
         * @param vx Velocity on the x axis
         */
        Velocity(float vx = 0, float vy = 0) : vx(vx), vy(vy) {}
    };
}
