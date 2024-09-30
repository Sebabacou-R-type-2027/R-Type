/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** create_ennemies
*/

#include "create_ennemies.hpp"

namespace ecs {
    void CreateEnnemies::create_enemy(Registry& registry,
                                    float x, float y,
                                    float speed,
                                    const std::string& texturePath,
                                    float hitboxSize,
                                    float minX, float maxX,
                                    float minY, float maxY,
                                    float angle, float radius, float angular_speed,
                                    State state)
    {
        auto enemy = registry.spawn_entity();
        auto &drawable = registry.emplace_component<ecs::Drawable>(enemy, texturePath, true, sf::IntRect(70, 10, 130, 124));

        float enemyWidth = drawable->sprite.getGlobalBounds().width;
        registry.emplace_component<ecs::Position>(enemy, x - enemyWidth, y);
        auto& hitbox = registry.emplace_component<ecs::Hitbox>(enemy, ecs::ShapeType::Rectangle, false);
        hitbox->rect = sf::RectangleShape(sf::Vector2f(hitboxSize, hitboxSize));
        registry.emplace_component<ecs::EnemyState>(enemy, state);
        registry.emplace_component<ecs::LoopMovement>(enemy, minX, maxX, minY, maxY, speed, angle, radius, angular_speed);
        registry.emplace_component<ecs::EntityType>(enemy, Type::Ennemy);
    }

    void CreateEnnemies::create_enemies(Registry& registry, sf::RenderWindow& window)
    {
        int max_y = window.getSize().y - 124;
        int max_x = window.getSize().x;

        for (int i = 0; i < 5; ++i) {
            std::cout << "Creating enemy " << i << std::endl;
            float yPosition = 100.0f + i * 100.0f;
            create_enemy(registry,
                        max_x, yPosition,
                        100.0f,
                        "assets/Ennemy.png",
                        50.0f,
                        0.0f, max_x,
                        0.0f, max_y,
                        0.0f, 0.0f, 0.0f,
                        State::Patrolling);
        }
    }
}