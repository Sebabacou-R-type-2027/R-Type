/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** draw_system
*/

#include "draw_system.hpp"
#include <iostream>

namespace ecs::systems {

    void DrawSystem::drawSprite(Drawable& drawable, Position& position, sf::RenderWindow& window) {
        if (drawable.sprite.getTexture()) {
            drawable.sprite.setPosition(position.x, position.y);
            if (drawable.texture_rect != sf::IntRect())
                drawable.sprite.setTextureRect(drawable.texture_rect);
            window.draw(drawable.sprite);
        }
    }

    void DrawSystem::drawText(Drawable& drawable, Position& position, sf::RenderWindow& window) {
        if (!drawable.text.getString().isEmpty()) {
            drawable.text.setPosition(position.x, position.y);
            window.draw(drawable.text);
        }
    }

    void DrawSystem::updateDrawable(Drawable& drawable, Position& position, sf::RenderWindow& window) {
        drawable.sprite.setTexture(drawable.texture);
        drawSprite(drawable, position, window);
        drawText(drawable, position, window);
    }

    void DrawSystem::update(Registry& registry, sf::RenderWindow& window) {
        auto &drawables = registry.get_components<Drawable>();
        auto &positions = registry.get_components<Position>();
        auto &animations = registry.get_components<Animation>();
        auto &hitboxes = registry.get_components<Hitbox>();

        for (std::size_t i = 0; i < drawables.size(); ++i) {
            if (drawables[i] && positions[i]) {
                updateDrawable(*drawables[i], *positions[i], window);
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
