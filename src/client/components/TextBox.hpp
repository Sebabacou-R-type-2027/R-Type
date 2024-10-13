/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Button
*/
#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <functional>

namespace ecs {
    struct TextBox {
        sf::RectangleShape rect;
        sf::Text text;
        std::function<void()> onClick;
        bool isPressed = false;

        sf::Color defaultColor = sf::Color::Blue;
        sf::Color hoverColor = sf::Color::Cyan;
        sf::Color clickColor = sf::Color::Green;

        TextBox(const sf::RectangleShape& shape, const sf::Text& btnText, std::function<void()> action)
            : rect(shape), text(btnText), onClick(action) {
            rect.setOrigin(rect.getSize().x / 2, rect.getSize().y / 2);
        }

    };
}