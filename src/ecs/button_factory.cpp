/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** button_factory
*/

#include "button_factory.hpp"

namespace ecs {

Button ButtonFactory::create_button(const std::string& text, const sf::Vector2f& position, const sf::Vector2f& size, const sf::Font& font, std::function<void()> action) {
    sf::RectangleShape shape(size);
    shape.setPosition(position);
    shape.setOrigin(size.x / 2, size.y / 2);

    sf::Text buttonText;
    buttonText.setFont(font);
    buttonText.setString(text);
    buttonText.setCharacterSize(24);
    buttonText.setFillColor(sf::Color::White);
    buttonText.setOrigin(buttonText.getGlobalBounds().width / 2, buttonText.getGlobalBounds().height / 2);
    buttonText.setPosition(position);

    return Button(shape, buttonText, action);
}

}
