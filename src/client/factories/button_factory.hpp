/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** button_factory
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include "components/Button.hpp"

namespace ecs {

/**
 * @brief Class that contains the logic of the button factory
 */
class ButtonFactory {
public:
    std::string currentText; ///< Current text of the button

    /**
     * @brief Create a button
     *
     * @param text contains the text of the button
     * @param position contains the position of the button
     * @param size contains the size of the button
     * @param font contains the font of the button
     * @param defaultColor contains the default color of the button
     * @param hoverColor contains the hover color of the button
     * @param clickColor contains the click color of the button
     * @param textColor contains the text color of the button
     * @param text_size contains the text size of the button
     * @param action contains the action of the button
     * @return Button
     */
    static Button create_button(
        const std::string& text,
        const sf::Vector2f& position,
        const sf::Vector2f& size,
        const sf::Font& font,
        const sf::Color& defaultColor = sf::Color::Blue,   // Default button color
        const sf::Color& hoverColor = sf::Color::Cyan,     // Default hover color
        const sf::Color& clickColor = sf::Color::Green,     // Default click color
        const sf::Color& textColor = sf::Color::White,      // Default text color
        const unsigned int text_size = 24,                  // Default text size
        std::function<void()> action = []() {});            // Default action (do nothing)

    /**
     * @brief Handle the mouse click
     * 
     * @param button contains the button
     * @param mousePos contains the mouse position
     */

    void handleTextEntered(sf::Uint32 unicode, const Button button);

};
}
