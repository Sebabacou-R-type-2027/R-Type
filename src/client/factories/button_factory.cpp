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

    if (!text.empty()) {
        buttonText.setString(text);
    } else {
        buttonText.setString("_");
    }


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


void ButtonFactory::handleTextEntered(sf::Uint32 unicode, const Button button) {
        sf::Text buttonText;

        if (button.isTextbox) {
            // On ne prend en charge que les caractères imprimables
            if (unicode >= 32 && unicode <= 126) {
                currentText += static_cast<char>(unicode);
            } else if (unicode == 8 && !currentText.empty()) { // Backspace
                currentText.pop_back();
            }

            // Mettre à jour le texte affiché
            if (currentText.empty()) {
                buttonText.setString("_");  // Curseur vide
            } else {
                buttonText.setString(currentText + "_");  // Affiche le texte avec un curseur
            }

            // Repositionner l'origine pour centrer le texte
            buttonText.setOrigin(buttonText.getGlobalBounds().width / 2, buttonText.getGlobalBounds().height / 2 + buttonText.getGlobalBounds().top);
        }
    }
}
