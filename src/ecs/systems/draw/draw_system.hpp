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
            void update(Registry& registry, sf::RenderWindow& window);
        private:
            void drawSprite(Drawable& drawable, Position& position, sf::RenderWindow& window);
            void drawText(Drawable& drawable, Position& position, sf::RenderWindow& window);
            void drawRectangle(Drawable& drawable, Position& position, sf::RenderWindow& window);

            void updateDrawable(Drawable& drawable, Position& position, sf::RenderWindow& window);
    };

}


