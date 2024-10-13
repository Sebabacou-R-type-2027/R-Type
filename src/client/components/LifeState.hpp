/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** LifeState
*/


#pragma once

namespace ecs {

    struct LifeState
    {
        bool isAlive;

        LifeState(bool isAlive = true) : isAlive(isAlive) {};
    };
};
