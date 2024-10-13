/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** LoopMovement
*/

#pragma once

namespace ecs {
    /**
     * @brief Class that contains the loop movement of an entity
     */
    struct LoopMovement {
        float min_x, max_x; ///< Min and max position on the x axis
        float min_y, max_y; ///< Min and max position on the y axis
        float speed; ///< Speed of the entity
        float angle; ///< Angle of the entity
        float radius; ///< Radius of the entity
        float angular_speed; ///< Angular speed of the entity

        /**
         * @brief Construct a new Loop Movement object
         * 
         * @param min_x contains the min position on the x axis
         * @param max_x contains the max position on the x axis
         * @param min_y contains the min position on the y axis
         * @param max_y contains the max position on the y axis
         * @param speed contains the speed of the entity
         * @param angle contains the angle of the entity
         * @param radius contains the radius of the entity
         * @param angular_speed contains the angular speed of the entity
         */
        LoopMovement(float min_x = 0.0f, float max_x = 0.0f, float min_y = 0.0f, float max_y = 0.0f,
                     float speed = 0.0f, float angle = 0.0f, float radius = 0.0f, float angular_speed = 0.0f)
            : min_x(min_x), max_x(max_x), min_y(min_y), max_y(max_y),
            speed(speed), angle(angle), radius(radius), angular_speed(angular_speed) {}
    };
}
