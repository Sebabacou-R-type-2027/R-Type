/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** entity
*/

#pragma once

#include <cstddef>

class Entity {
    public:
        explicit Entity(std::size_t id) :  _id(id) {}
        operator std::size_t() const { return _id; }

    private:
        std::size_t _id;
};
