/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** draw_system
*/

#include "draw_system.hpp"

namespace ecs::systems {

void DrawSystem::update(Registry& registry, sf::RenderWindow& window, sf::CircleShape& playerShape) {
    auto& positions = registry.get_components<Position>();

    for (std::size_t i = 0; i < positions.size(); ++i) {
        if (positions[i]) {
            playerShape.setPosition(positions[i]->x, positions[i]->y);
            window.draw(playerShape);
        }
    }
}

void DrawSystem::update(Registry& registry, sf::RenderWindow& window, sf::Sprite& playerSprite) {
    auto& positions = registry.get_components<Position>();

    for (std::size_t i = 0; i < positions.size(); ++i) {
        if (positions[i]) {
            playerSprite.setPosition(positions[i]->x, positions[i]->y);
            window.draw(playerSprite);
        }
    }
}

}
