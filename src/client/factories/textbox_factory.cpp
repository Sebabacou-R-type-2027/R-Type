/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** button_factory
*/

#include "textbox_factory.hpp"

namespace ecs {

TextBox TextBoxFactory::create_textbox(
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
    shape.setFillColor(defaultColor);
    shape.setPosition(position);

    sf::Text buttonText;
    buttonText.setFont(font);
    buttonText.setString(text.empty() ? "_" : text);
    buttonText.setCharacterSize(text_size);
    buttonText.setFillColor(textColor);
    buttonText.setOrigin(buttonText.getGlobalBounds().width / 2, buttonText.getGlobalBounds().height / 2 + buttonText.getGlobalBounds().top);
    buttonText.setPosition(position);

    TextBox text_box(shape, buttonText, action);
    text_box.defaultColor = defaultColor;
    text_box.hoverColor = hoverColor;
    text_box.clickColor = clickColor;

    return text_box;
}


    void TextBoxFactory::handleTextEntered(sf::Uint32 unicode, TextBox& textBox) {
        if (textBox.isPressed) {
            sf::Text& buttonText = textBox.text;

            if (unicode >= 32 && unicode <= 126) {
                currentText += static_cast<char>(unicode);
            } else if (unicode == 8 && !currentText.empty()) { // Backspace
                currentText.pop_back();
            }

            buttonText.setString(currentText.empty() ? "_" : currentText + "_");
            buttonText.setOrigin(buttonText.getGlobalBounds().width / 2, buttonText.getGlobalBounds().height / 2 + buttonText.getGlobalBounds().top);
        }
    }

}