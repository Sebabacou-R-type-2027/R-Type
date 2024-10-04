/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** button_factory
*/

#include "button_factory.hpp"

namespace ecs {

Button ButtonFactory::create_button(
    const std::string& text,
    const sf::Vector2f& position,
    const sf::Vector2f& size,
    const sf::Font& font,
    const sf::Color& defaultColor,
    const sf::Color& hoverColor,
    const sf::Color& clickColor,
    const sf::Color& textColor,
    const unsigned int text_size,
    std::function<void()> action) {

    sf::RectangleShape shape(size);
    shape.setFillColor(defaultColor); // Set the button's default color
    shape.setPosition(position);

    sf::Text buttonText;
    buttonText.setFont(font);
    buttonText.setString(text);
    buttonText.setCharacterSize(text_size);
    buttonText.setFillColor(textColor);
    buttonText.setOrigin(buttonText.getGlobalBounds().width / 2, buttonText.getGlobalBounds().height / 2 + buttonText.getGlobalBounds().top);
    buttonText.setPosition(position);

    // Create the Button object with colors
    Button button(shape, buttonText, action);
    button.defaultColor = defaultColor;
    button.hoverColor = hoverColor;
    button.clickColor = clickColor;

    return button;
}

}
