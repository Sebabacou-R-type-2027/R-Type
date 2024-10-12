/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** draw_system
*/

#include "draw_system.hpp"
#include <iostream>

namespace ecs::systems {

void DrawSystem::update(Registry& registry, sf::RenderWindow& window) {
    auto &drawables = registry.get_components<Drawable>();
    auto &positions = registry.get_components<Position>();
    auto &animations = registry.get_components<Animation>();
    auto &hitboxes = registry.get_components<Hitbox>();

    for (std::size_t i = 0; i < drawables.size() && i < positions.size(); ++i) {
        if (drawables[i] && positions[i]) {
            sf::Sprite sprite;
            sprite.setTexture(drawables[i]->texture);
            if (drawables[i]->texture_rect != sf::IntRect())
                sprite.setTextureRect(drawables[i]->texture_rect);
            sprite.setPosition(positions[i]->x, positions[i]->y);
            window.draw(sprite);
        }
    }
    for (std::size_t i = 0; i < animations.size() && i < positions.size(); ++i) {
        if (animations[i] && positions[i]) {
            animations[i]->sprite.setPosition(positions[i]->x, positions[i]->y);
            window.draw(animations[i]->sprite);
        }
    }
    sf::Font font;
    if (!font.loadFromFile("assets/fonts/NimbusSanL-Bold.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
        return;
    }
    for (std::size_t i = 0; i < hitboxes.size() && i < positions.size(); ++i) {
        if (hitboxes[i] && positions[i] && hitboxes[i]->debugdisplay) {
            auto &rect = hitboxes[i]->rect;
            rect.setFillColor(sf::Color::Transparent);
            rect.setPosition(positions[i]->x, positions[i]->y);
            sf::Text text(std::to_string(i), font, 12);
            text.setPosition(positions[i]->x, positions[i]->y);
            window.draw(text);
            window.draw(rect);
        }
    }
}

}
