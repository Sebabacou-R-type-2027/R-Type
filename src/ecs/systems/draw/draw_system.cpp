/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** draw_system
*/

#include "draw_system.hpp"
#include <iostream>

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
            sf::Text tempText;
            tempText.setFont(drawable.font);
            tempText.setCharacterSize(drawable.text.getCharacterSize());
            tempText.setString(drawable.text.getString());
            tempText.setFillColor(drawable.text.getFillColor());
            tempText.setStyle(drawable.text.getStyle());
            tempText.setPosition(position.x, position.y);
            window.draw(tempText);
        }
    }

    void DrawSystem::drawRectangle(Drawable& drawable, Position& position, sf::RenderWindow& window) {
        if (drawable.rectangle.getSize() != sf::Vector2f(0, 0) && drawable.visible) {
            drawable.rectangle.setPosition(position.x, position.y);
            window.draw(drawable.rectangle);
        }
    }

    void DrawSystem::updateDrawable(Drawable& drawable, Position& position, sf::RenderWindow& window) {
        drawable.sprite.setTexture(drawable.texture);
        drawSprite(drawable, position, window);
        drawText(drawable, position, window);
        drawRectangle(drawable, position, window);
    }

    void DrawSystem::update(Registry& registry, sf::RenderWindow& window) {
        auto& drawables = registry.get_components<Drawable>();
        auto& positions = registry.get_components<Position>();
        auto& animations = registry.get_components<Animation>();

        for (std::size_t i = 0; i < drawables.size() && i < positions.size(); ++i) {
            if (drawables[i] && positions[i]) {
                updateDrawable(*drawables[i], *positions[i], window);
            }
        }

        for (std::size_t i = 0; i < animations.size() && i < positions.size(); ++i) {
            if (animations[i] && positions[i]) {
                Animation& animation = *animations[i];
                Position& position = *positions[i];
                animation.sprite.setPosition(position.x, position.y);
                window.draw(animation.sprite);
            }
        }
    }

}
