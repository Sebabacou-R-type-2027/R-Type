/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** animation_system
*/

#pragma once

#include "registry.hpp"
#include <SFML/Graphics.hpp>

namespace ecs::systems {

    class AnimationSystem {
        public:
            AnimationSystem() = default;
            void update(Registry& registry, float deltaTime, sf::RenderWindow& window);
        private:
    };

}