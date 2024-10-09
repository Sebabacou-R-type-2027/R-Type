/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Handle_enemies_wave
*/

#pragma once

#include "registry.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

namespace ecs {
    class HandleEnemiesWave {
        public:
            void update(Registry& registry);
    };
}
