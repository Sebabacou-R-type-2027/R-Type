/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Collision_state
*/

#pragma once

namespace ecs {
    /**
     * @brief Class that contains the collision state of an entity
     */
    struct CollisionState {
        bool active;

        /**
         * @brief Construct a new Collision State object
         *
         * @param active If the entity is colliding
         */
        CollisionState (bool active = false)
            : active(active) {}
    };
}
