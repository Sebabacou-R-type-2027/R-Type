/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Entity_type
*/


#pragma once

namespace ecs {
    /**
     * @brief Enum to define the type of an entity
     */
    enum class Type {
        Player,
        Ennemy,
        Bullet,
        Button,
        PowerUp,
        EnnemyBullet,
        ChargedBullet,
        BulletHeal
    };

    /**
     * @brief Class that contains the type of an entity
     */
    struct EntityType {
        Type current_type;

        /**
         * @brief Construct a new Entity Type object
         *
         * @param type Type of the entity
         */
        EntityType(Type type = Type::Player)
            : current_type(type) {}
    };
}
