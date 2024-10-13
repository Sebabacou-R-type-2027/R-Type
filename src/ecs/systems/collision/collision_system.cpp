/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** collision_system
*/

#include "collision_system.hpp"
#include <iostream>

namespace ecs::systems {

    bool isColliding(const Position& pos1, const Hitbox& hitbox1, const Position& pos2, const Hitbox& hitbox2) {
        return !(pos1.x + hitbox1.rect.getSize().x <= pos2.x ||
                pos1.x >= pos2.x + hitbox2.rect.getSize().x ||
                pos1.y + hitbox1.rect.getSize().y <= pos2.y ||
                pos1.y >= pos2.y + hitbox2.rect.getSize().y);
    }

    void CollisionSystem::update(Registry& registry, sf::RenderWindow& window) {
        auto &positions = registry.get_components<Position>();
        auto &hitboxes = registry.get_components<Hitbox>();
        auto &collision_states = registry.get_components<CollisionState>();

        for (std::size_t i = 0; i < positions.size() && i < hitboxes.size() && i < collision_states.size(); ++i) {
            if (!positions[i] ||
            !hitboxes[i] ||
            !collision_states[i]) continue;

            Position& pos1 = *positions[i];
            Hitbox& hitbox1 = *hitboxes[i];
            for (std::size_t j = 0; j < positions.size() && j < hitboxes.size() && j < collision_states.size(); ++j) {
                if (i == j || !positions[j] || !hitboxes[j] || !collision_states[j]) continue;

                Position& pos2 = positions[j].value();
                Hitbox& hitbox2 = *hitboxes[j];
                if (isColliding(pos1, hitbox1, pos2, hitbox2)) {
                    collision_states[i]->active = true;
                    collision_states[j]->active = true;
                }
            }
        }
    }
}