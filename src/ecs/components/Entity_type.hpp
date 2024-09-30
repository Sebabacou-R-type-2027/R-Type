/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Entity_type
*/


#pragma once

namespace ecs {
    enum class Type {
        Player,
        Ennemy,
        Bullet,
        Button
    };

    struct EntityType {
        Type current_type;

        EntityType(Type type = Type::Player)
            : current_type(type) {}
    };
}
