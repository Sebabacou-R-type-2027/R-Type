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

            // Scale factor for hover and click
            float scaleFactor = containsMouse ? 1.1f : 1.0f;

            // Scale button
            buttons[i]->rect.setScale(scaleFactor, scaleFactor);

            // Handle click
            if (containsMouse) {
                buttons[i]->rect.setFillColor(sf::Color::Cyan); // Hover color
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    buttons[i]->rect.setScale(0.9f, 0.9f); // Scale down on click
                    if (!buttons[i]->isPressed) {
                        buttons[i]->isPressed = true; // Set pressed state
                        buttons[i]->onClick(); // Call action
                    }
                } else {
                    buttons[i]->isPressed = false; // Reset pressed state when released
                }
            } else {
                buttons[i]->rect.setFillColor(sf::Color::Blue); // Default color
                buttons[i]->isPressed = false; // Reset pressed state
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
