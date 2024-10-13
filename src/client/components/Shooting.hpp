/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Shooting
*/

#pragma once

namespace ecs {
    /**
     * @brief Class that contains the shooting of an entity
     */
    struct Shooting {
        float cooldown;
        float lastShot;

        /**
         * @brief Construct a new Shooting object
         *
         */
        Shooting(float cooldown = 1.0f, float lastShot = 0.0f)
            : cooldown(cooldown), lastShot(lastShot) {};
    };
}