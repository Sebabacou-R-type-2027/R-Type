module;

#if __cpp_lib_modules < 202207L
#endif
#include <SFML/Graphics.hpp>


export module game.systems.button_system;
import game.components.buttons;
#if __cpp_lib_modules >= 202207L
import std;
#endif
import ecs;

export namespace game::systems {
    void handle_button(ecs::entity_container &ec, game::components::button &button) noexcept
    {
        auto window = ec.get_entity(button.window).and_then([&ec](auto e){
            return ec.get_entity_component<ecs::components::gui::window>(e);
        });
        if (!window)
            return;

        sf::Vector2i mousePos = sf::Mouse::getPosition(window->get().window);

        bool containsMouse = button.rect.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));

        float scaleFactor = containsMouse ? 1.05f : 1.0f;

        button.rect.setScale(scaleFactor, scaleFactor);
        button.text.setScale(scaleFactor, scaleFactor);

        if (containsMouse) {
            button.rect.setFillColor(button.clickColor);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                button.rect.setScale(0.95f, 0.95f);
                button.text.setScale(0.95f, 0.95f);
                button.rect.setFillColor(button.clickColor);
                if (!button.isPressed) {
                    button.isPressed = true;
                    button.onClick();
                }
            } else {
                button.isPressed = false;
            }
        } else {
            button.rect.setFillColor(button.defaultColor);
            button.isPressed = false;
        }
    }

    void render_button(ecs::entity_container &ec, game::components::button &button) noexcept {
        auto window = ec.get_entity(button.window).and_then([&ec](auto e){
            return ec.get_entity_component<ecs::components::gui::window>(e);
        });
        if (!window)
            return;

        window->get().window.draw(button.rect);
        window->get().window.draw(button.text);
    }
}
