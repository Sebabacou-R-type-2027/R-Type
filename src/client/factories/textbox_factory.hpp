/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** button_factory
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include "components/TextBox.hpp"

namespace ecs {

class TextBoxFactory {
public:
    std::string currentText;

    static TextBox create_textbox(
        const std::string& text,
        const sf::Vector2f& position,
        const sf::Vector2f& size,
        const sf::Font& font,
        const sf::Color& defaultColor = sf::Color::Blue,   // Default button color
        const sf::Color& hoverColor = sf::Color::Cyan,     // Default hover color
        const sf::Color& clickColor = sf::Color::Green,     // Default click color
        const sf::Color& textColor = sf::Color::White,      // Default text color
        const unsigned int text_size = 24,
        std::function<void()> action = []() {});            // Default action (do nothing)


    void handleTextEntered(sf::Uint32 unicode, TextBox &textBox);
};
}
