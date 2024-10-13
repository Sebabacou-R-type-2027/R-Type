/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** entity
*/

#pragma once

#include <cstddef>

namespace ecs {

class Entity {
    public:
        explicit Entity(std::size_t id) : _id(id) {}
        operator std::size_t() const { return _id; }

        bool is_valid() const { return _id != -1; }
    private:
        std::size_t _id;
};

}