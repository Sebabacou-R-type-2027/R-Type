/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Drawable
*/

#pragma once

#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <stdexcept>

namespace ecs {
    /**
     * @brief Class that contains the drawable component
     */
    struct Drawable {
        bool visible;
        sf::Sprite sprite;
        sf::Texture texture;
        sf::IntRect texture_rect;
        sf::Color color;
        sf::Text text;
        sf::Font font;
        sf::RectangleShape rectangle; // Add a rectangle shape for drawing

        /**
         * @brief Construct a new Drawable object
         *
         * @param texturePath Path to the texture
         * @param visible If the drawable is visible
         * @param texture_rect Rectangle of the texture
         * @param color Color of the drawable
         */
        // Constructor for textures
        Drawable(const std::string& texturePath, bool visible = true,
                 sf::IntRect texture_rect = sf::IntRect(), sf::Color color = sf::Color::White)
            : visible(visible), texture_rect(texture_rect), color(color) {
            if (!texture.loadFromFile(texturePath)) {
                throw std::runtime_error("Failed to load texture from " + texturePath);
            }
            sprite.setTexture(texture);
            if (texture_rect != sf::IntRect()) {
                sprite.setTextureRect(texture_rect);
            }
        }

        /**
         * @brief Construct a new Drawable object
         *
         * @param fontPath Path to the font
         * @param displayText Text to display
         * @param characterSize Size of the text
         * @param textColor Color of the text
         * @param visible If the drawable is visible
         */
        // Constructor for text
        Drawable(const std::string& fontPath, const std::string& displayText,
                 unsigned int characterSize = 24, sf::Color textColor = sf::Color::White,
                 bool visible = true)
            : visible(visible), color(textColor), text(displayText, font, characterSize) {
            if (!font.loadFromFile(fontPath)) {
                throw std::runtime_error("Failed to load font from " + fontPath);
            }
            text.setFont(font);
            text.setFillColor(textColor);
        }

        /**
         * @brief Construct a new Drawable object
         *
         * @param width Width of the rectangle
         * @param height Height of the rectangle
         * @param fillColor Color of the rectangle
         * @param outlineColor Color of the outline
         * @param visible If the drawable is visible
         */
        // Constructor for rectangle with custom bounds
        Drawable(float width, float height, sf::Color fillColor = sf::Color::White,
                  sf::Color outlineColor = sf::Color::Black, bool visible = true)
            : visible(visible), color(fillColor) {
            rectangle.setSize(sf::Vector2f(width, height));
            rectangle.setFillColor(fillColor);
            rectangle.setOutlineThickness(1); // Set a default outline thickness
            rectangle.setOutlineColor(outlineColor); // Set the outline color
        }

        /**
         * @brief Construct a new Drawable object
         *
         * @param displayText Text to display
         * @param characterSize Size of the text
         */
        // Method to update the text
        void setText(const std::string& displayText, unsigned int characterSize = 24) {
            text.setString(displayText);
            text.setCharacterSize(characterSize);
            text.setFont(font); // Ensure the font is set when changing text
        }

        /**
         * @brief Construct a new Drawable object
         *
         * @param width Width of the rectangle
         * @param height Height of the rectangle
         * @param fillColor Color of the rectangle
         * @param outlineColor Color of the outline
         */
        // Method to set rectangle properties
        void setRectangleProperties(float width, float height, sf::Color fillColor,
                                    sf::Color outlineColor = sf::Color::Black) {
            rectangle.setSize(sf::Vector2f(width, height));
            rectangle.setFillColor(fillColor);
            rectangle.setOutlineColor(outlineColor);
        }
    };
}