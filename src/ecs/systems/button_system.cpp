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
            bool containsMouse = buttons[i]->rect.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));

            if (containsMouse && sf::Mouse::isButtonPressed(sf::Mouse::Left) && !buttons[i]->isPressed) {
                buttons[i]->isPressed = true; // Set the pressed state
                buttons[i]->onClick(); // Call the button's action
            } else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                buttons[i]->isPressed = false; // Reset when mouse is released
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
