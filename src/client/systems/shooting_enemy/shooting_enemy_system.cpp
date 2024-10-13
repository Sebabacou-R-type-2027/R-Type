/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** shooting_enemy_system
*/

#include "shooting_enemy_system.hpp"
#include <iostream>

namespace ecs::systems {

    void ShootingEnemySystem::update(Registry& registry, sf::RenderWindow& window) {
        auto &positions = registry.get_components<Position>();
        auto &hitboxes = registry.get_components<Hitbox>();
        auto &shooting = registry.get_components<Shooting>();

        for (std::size_t i = 0; i < positions.size() && i < hitboxes.size() && i < shooting.size(); ++i) {
            if (!positions[i] ||
            !hitboxes[i] ||
            !shooting[i]) continue;

            Position& pos1 = *positions[i];
            Hitbox& hitbox1 = *hitboxes[i];
            Shooting& shooting1 = *shooting[i];
            if (shooting1.lastShot + shooting1.cooldown < clock.getElapsedTime().asSeconds()) {
                shooting1.lastShot = clock.getElapsedTime().asSeconds();

                Entity bullet = registry.spawn_entity();

                registry.emplace_component<Position>(bullet, pos1.x + hitbox1.rect.getSize().x - 80, pos1.y + hitbox1.rect.getSize().y);

                registry.emplace_component<Velocity>(bullet, -10.0f, 0.0f);

                registry.emplace_component<Bullet>(bullet);
                auto& draw = registry.emplace_component<Drawable>(bullet, "assets/Bullets/02.png");
                auto& hit = registry.emplace_component<ecs::Hitbox>(bullet, ecs::ShapeType::Rectangle, false);
                hit->rect = sf::RectangleShape(sf::Vector2f(draw->sprite.getGlobalBounds().width, draw->sprite.getGlobalBounds().height));
                hit->rect.setOutlineColor(sf::Color::Red);
                hit->rect.setOutlineThickness(1.0f);
                registry.emplace_component<EntityType>(bullet, Type::EnnemyBullet);
                registry.emplace_component<CollisionState>(bullet, false);
                registry.emplace_component<LifeState>(bullet, true);
            }
        }
    }
}
