module;

#if __cpp_lib_modules < 202207L
#include <functional>
#endif
#include <SFML/Graphics.hpp>
export module game:components.buttons;

#if __cpp_lib_modules >= 202207L
import std;
#endif
import ecs;

export namespace game::components {
    struct button {
        sf::RectangleShape rect;
        sf::Text text;
        std::function<void()> onClick;
        bool isPressed = false;
        ecs::entity window;
        sf::Color defaultColor = sf::Color::Blue;
        sf::Color hoverColor = sf::Color::Cyan;
        sf::Color clickColor = sf::Color::Green;

        button(const sf::RectangleShape& shape, const sf::Text& btnText, std::function<void()> action, const ecs::entity& win)
            : rect(shape), text(btnText), onClick(action), window(win) {
            rect.setOrigin(rect.getSize().x / 2, rect.getSize().y / 2);
        }
    };
}
