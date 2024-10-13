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

    /**
     * @brief Class that contains all the logic of the draw system
     */
    class DrawSystem {
        public:
            /**
             * @brief Update the draw system
             *
             * @param registry contains all the logic of the components
             * @param window contains the window of the game
             */
            void update(Registry& registry, sf::RenderWindow& window);
        private:
            /**
             * @brief Draw a sprite
             * 
             * @param drawable contains the sprite to draw
             * @param position contains the position of the sprite
             * @param window contains the window of the game
             */
            void drawSprite(Drawable& drawable, Position& position, sf::RenderWindow& window);
            /**
             * @brief Draw a text
             * 
             * @param drawable contains the text to draw
             * @param position contains the position of the text
             * @param window contains the window of the game
             */
            void drawText(Drawable& drawable, Position& position, sf::RenderWindow& window);
            /**
             * @brief Draw a rectangle
             * 
             * @param drawable contains the rectangle to draw
             * @param position contains the position of the rectangle
             * @param window contains the window of the game
             */
            void drawRectangle(Drawable& drawable, Position& position, sf::RenderWindow& window);
            /**
             * @brief Draw a circle
             * 
             * @param drawable contains the circle to draw
             * @param position contains the position of the circle
             * @param window contains the window of the game
             */
            void updateDrawable(Drawable& drawable, Position& position, sf::RenderWindow& window);
    };

}

