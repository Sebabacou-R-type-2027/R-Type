/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** button_factory
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include "components.hpp"

namespace ecs {

class ButtonFactory {
public:
    static Button create_button(const std::string& text, const sf::Vector2f& position, const sf::Vector2f& size, const sf::Font& font, std::function<void()> action);
};

}
