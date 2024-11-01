/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Controllable
*/

#pragma once

namespace ecs {
    /**
     * @brief Class that contains the controllable component
     */
    struct Controllable {
        bool active;
        float speed;

        /**
         * @brief Construct a new Controllable object
         *
         * @param active If the entity is controllable
         * @param speed Speed of the entity
         */
        Controllable(bool active = true, float speed = 100.0f)
            : active(active), speed(speed) {}
    };
}
