/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** draw_system
*/

#include "draw_system.hpp"
#include <iostream>

namespace ecs::systems {

// void DrawSystem::update(Registry& registry, sf::RenderWindow& window, sf::CircleShape& playerShape) {
//     auto positions = registry.get_components<Position>();

//     for (std::size_t i = 0; i < positions.size(); ++i) {
//         if (positions[i]) {
//             playerShape.setPosition(positions[i]->get().x, positions[i]->get().y);
//             window.draw(playerShape);
//         }
//     }
// }

// void DrawSystem::update(Registry& registry, sf::RenderWindow& window, sf::Sprite& playerSprite) {
//     auto positions = registry.get_components<Position>();

//     for (std::size_t i = 0; i < positions.size(); ++i) {
//         if (positions[i]) {
//             playerSprite.setPosition(positions[i]->get().x, positions[i]->get().y);
//             window.draw(playerSprite);
//         }
//     }
// }

void DrawSystem::update(Registry& registry, sf::RenderWindow& window) {
    auto drawables = registry.get_components<Drawable>();
    auto positions = registry.get_components<Position>();
    auto animations = registry.get_components<Animation>();
    auto hitboxes = registry.get_components<Hitbox>();

    for (std::size_t i = 0; i < drawables.size(); ++i) {
        if (drawables[i] && positions[i]) {
            sf::Sprite sprite;
            sprite.setTexture(drawables[i]->get().texture);
            if (drawables[i]->get().texture_rect != sf::IntRect())
                sprite.setTextureRect(drawables[i]->get().texture_rect);
            sprite.setPosition(positions[i]->get().x, positions[i]->get().y);
            window.draw(sprite);
        }
    }
    for (std::size_t i = 0; i < animations.size(); ++i) {
        if (animations[i] && positions[i]) {
            window.draw(animations[i]->get().sprite);
        }
    }
    sf::Font font;
    if (!font.loadFromFile("assets/fonts/NimbusSanL-Bol.otf")) {
        std::cerr << "Failed to load font!" << std::endl;
        return;
    }
    for (std::size_t i = 0; i < hitboxes.size(); ++i) {
        if (hitboxes[i] && positions[i] && hitboxes[i]->get().debugdisplay) {
            auto &rect = hitboxes[i]->get().rect;
            rect.setFillColor(sf::Color::Transparent);
            rect.setPosition(positions[i]->get().x, positions[i]->get().y);
            sf::Text text(std::to_string(i), font, 12);
            text.setPosition(positions[i]->get().x, positions[i]->get().y);
            window.draw(text);
            window.draw(rect);
        }
    }
}

}
