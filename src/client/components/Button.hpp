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
    /**
     * @brief Class that contains the button of an entity
     */
    struct Button {
        sf::RectangleShape rect; ///< Rectangle of the button
        sf::Text text; ///< Text of the button
        std::function<void()> onClick; ///< Action of the button
        bool isPressed = false; ///< If the button is pressed

        sf::Color defaultColor = sf::Color::Blue; ///< Default color of the button
        sf::Color hoverColor = sf::Color::Cyan; ///< Hover color of the button
        sf::Color clickColor = sf::Color::Green; ///< Click color of the button

        /**
         * @brief Construct a new Button object
         * 
         * @param shape contains the shape of the button
         * @param btnText contains the text of the button
         * @param action contains the action of the button
         */
        Button(const sf::RectangleShape& shape, const sf::Text& btnText, std::function<void()> action)
            : rect(shape), text(btnText), onClick(action) {
            rect.setOrigin(rect.getSize().x / 2, rect.getSize().y / 2);
        }

    };
}