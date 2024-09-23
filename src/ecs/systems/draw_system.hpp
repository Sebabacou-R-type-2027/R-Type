/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** draw_system
*/

#pragma once

#include "registry.hpp"
#include <SFML/Graphics.hpp>

namespace ecs::systems {

    class DrawSystem {
        public:
            void update(Registry& registry, sf::RenderWindow& window, sf::CircleShape& playerShape);
            void update(Registry& registry, sf::RenderWindow& window, sf::Sprite& playerSprite);
    };

}


