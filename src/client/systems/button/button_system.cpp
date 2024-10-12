// button_system.cpp

#include "button_system.hpp"

namespace ecs::systems {

void ButtonSystem::update(Registry& registry, sf::RenderWindow& window) {
    auto &buttons = registry.get_components<Button>();
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    for (std::size_t i = 0; i < buttons.size(); ++i) {
        if (buttons[i]) {
            bool containsMouse = buttons[i]->rect.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));

            // Scale factor for hover
            float scaleFactor = containsMouse ? 1.1f : 1.0f;

            // Scale button and text
            buttons[i]->rect.setScale(scaleFactor, scaleFactor);
            buttons[i]->text.setScale(scaleFactor, scaleFactor);

            // Handle click
            if (containsMouse) {
                buttons[i]->rect.setFillColor(buttons[i]->hoverColor); // Use hover color
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    buttons[i]->rect.setScale(0.9f, 0.9f); // Scale down on click
                    buttons[i]->text.setScale(0.9f, 0.9f); // Scale down on click
                    buttons[i]->rect.setFillColor(buttons[i]->clickColor); // Use click color
                    if (!buttons[i]->isPressed) {
                        buttons[i]->isPressed = true; // Set pressed state
                        buttons[i]->onClick(); // Call action
                    }
                } else {
                    buttons[i]->isPressed = false; // Reset pressed state when released
                }
            } else {
                buttons[i]->rect.setFillColor(buttons[i]->defaultColor); // Use default color
                buttons[i]->isPressed = false; // Reset pressed state
            }
        }
    }
}

void ButtonSystem::render(Registry& registry, sf::RenderWindow& window) {
    auto buttons = registry.get_components<Button>();
    for (std::size_t i = 0; i < buttons.size(); ++i) {
        if (buttons[i]) {
            window.draw(buttons[i]->rect);
            window.draw(buttons[i]->text);
        }
    }
}

}
