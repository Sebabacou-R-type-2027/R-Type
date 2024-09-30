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
        auto& positions = registry.get_components<Position>();
        auto& hitboxes = registry.get_components<Hitbox>();
        auto& controllable = registry.get_components<Controllable>();
        auto& entities = registry.get_components<EntityType>();

        for (std::size_t i = 0; i < positions.size(); ++i) {
            if (!positions[i] || !hitboxes[i] || !controllable[i] || !entities[i]) continue;

            Position& pos1 = *positions[i];
            Hitbox& hitbox1 = *hitboxes[i];
            Controllable& controllable1 = *controllable[i];
            EntityType& entity1 = *entities[i];

            for (std::size_t j = 0; j < positions.size(); ++j) {
                if (i == j || !positions[j] || !hitboxes[j]) continue;

                Position& pos2 = *positions[j];
                Hitbox& hitbox2 = *hitboxes[j];
                EntityType& entity2 = *entities[j];

                if (entity1.current_type == Type::Bullet && entity2.current_type == Type::Ennemy) {
                    if (isColliding(pos1, hitbox1, pos2, hitbox2)) {
                        registry.kill_entity(registry.entity_from_index(j));
                    }
                } else
                if (isColliding(pos1, hitbox1, pos2, hitbox2)) {
                    // std::cout << "Collision detected between entity " << i << " and entity " << j << std::endl;
                }
            }
        }
    }
}