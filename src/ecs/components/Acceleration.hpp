/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Acceleration
*/

#pragma once

namespace ecs {
    struct Acceleration {
        float ax = 0;
        float ay = 0;

        Acceleration(float ax = 0, float ay = 0) : ax(ax), ay(ay) {}
    };
}
