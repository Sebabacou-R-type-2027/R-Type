/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** button_system
*/

#pragma once

#include "ecs/registry.hpp"
#include "ecs/components/Button.hpp"
#include <SFML/Graphics.hpp>

namespace ecs::systems {

    class ButtonSystem {
        public:
            void update(Registry& registry, sf::RenderWindow& window);
            void render(Registry& registry, sf::RenderWindow& window);
    };

}