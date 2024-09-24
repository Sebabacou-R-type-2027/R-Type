#include "button_system.hpp"

namespace ecs::systems {

void ButtonSystem::update(Registry& registry, sf::RenderWindow& window) {
    auto& buttons = registry.get_components<Button>();
    auto& drawables = registry.get_components<Drawable>();
    auto& positions = registry.get_components<Position>();

    for (std::size_t i = 0; i < buttons.size(); ++i) {
        if (buttons[i] && drawables[i] && positions[i]) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            if (mousePos.x >= positions[i]->x && mousePos.x <= positions[i]->x + drawables[i]->size &&
                mousePos.y >= positions[i]->y && mousePos.y <= positions[i]->y + drawables[i]->size) {

                buttons[i]->isHovered = true;

                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    buttons[i]->isClicked = true;
                    if (buttons[i]->onClick) {
                        buttons[i]->onClick();
                    }
                }
            } else {
                buttons[i]->isHovered = false;
            }
        }
    }
}

}
