/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** button_system
*/

#include "button_system.hpp"

namespace ecs::systems {

void ButtonSystem::update(Registry& registry, sf::RenderWindow& window) {
    auto& buttons = registry.get_components<Button>();
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    for (std::size_t i = 0; i < buttons.size(); ++i) {
        if (buttons[i]) {
            if (buttons[i]->rect.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && buttons[i]->onClick) {
                    buttons[i]->onClick(); // Call the button's action
                }
            }
        }
    }
}

void ButtonSystem::render(Registry& registry, sf::RenderWindow& window) {
    auto& buttons = registry.get_components<Button>();
    for (std::size_t i = 0; i < buttons.size(); ++i) {
        if (buttons[i]) {
            window.draw(buttons[i]->rect);
            window.draw(buttons[i]->text);
        }
    }
}

}
