/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Drawable
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <stdexcept>

namespace ecs {
    struct Drawable {
        bool visible;
        sf::Sprite sprite;
        sf::Texture texture;
        sf::IntRect texture_rect;
        sf::Color color;
        sf::Vector2f size;
        std::string shape;

        Drawable(const std::string& texturePath, bool visible = true,
                 sf::IntRect texture_rect = sf::IntRect(), sf::Color color = sf::Color::White,
                 sf::Vector2f size = sf::Vector2f(0.0f, 0.0f), const std::string& shape = "")
            : visible(visible), texture(), texture_rect(texture_rect),
              color(color), size(size), shape(shape) {
            if (!texture.loadFromFile(texturePath)) {
                throw std::runtime_error("Failed to load texture from " + texturePath);
            }
            sprite.setTexture(texture);
            if (texture_rect != sf::IntRect())
                sprite.setTextureRect(texture_rect);
        }
    };
}
