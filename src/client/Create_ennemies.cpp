/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** create_ennemies
*/

#include "Create_ennemies.hpp"

namespace ecs {
    void CreateEnnemies::create_classic_enemy(Registry& registry,
                                    float x, float y,
                                    float speed,
                                    const std::string& texturePath,
                                    float minX, float maxX,
                                    float minY, float maxY,
                                    float angle, float radius, float angular_speed,
                                    State state)
    {
        auto enemy = registry.spawn_entity();
        std::cout << "Creating enemy entity #" << static_cast<std::size_t>(enemy) << std::endl;
        auto &animation = registry.emplace_component<ecs::Animation>(enemy, texturePath, 8, 1, 0.2f);
        float enemyWidth = animation->imageSize.x;
        registry.emplace_component<ecs::Position>(enemy, x - enemyWidth, y);

        auto& hitbox = registry.emplace_component<ecs::Hitbox>(enemy, ecs::ShapeType::Rectangle, false, true);
        hitbox->rect = sf::RectangleShape(sf::Vector2f(animation->imageSize.x, animation->imageSize.y));
        hitbox->rect.setOutlineColor(sf::Color::Red);
        hitbox->rect.setOutlineThickness(1.0f);

        registry.emplace_component<ecs::EnemyState>(enemy, state);
        registry.emplace_component<ecs::LoopMovement>(enemy, minX, maxX, minY, maxY, speed, angle, radius, angular_speed);
        registry.emplace_component<ecs::EntityType>(enemy, Type::Ennemy);
        registry.emplace_component<ecs::CollisionState>(enemy, false);
        registry.emplace_component<ecs::LifeState>(enemy, true);
    }

    void CreateEnnemies::create_shooting_enemy(Registry& registry,
                                    float x, float y,
                                    float speed,
                                    const std::string& texturePath,
                                    float minX, float maxX,
                                    float minY, float maxY,
                                    float angle, float radius, float angular_speed,
                                    State state)
    {
        auto enemy = registry.spawn_entity();
        std::cout << "Creating enemy entity #" << static_cast<std::size_t>(enemy) << std::endl;
        auto &draw = registry.emplace_component<ecs::Drawable>(enemy, texturePath);
        float enemyWidth = draw->sprite.getGlobalBounds().width;
        registry.emplace_component<ecs::Position>(enemy, x - enemyWidth, y);

        auto& hitbox = registry.emplace_component<ecs::Hitbox>(enemy, ecs::ShapeType::Rectangle, false, true);
        hitbox->rect = sf::RectangleShape(sf::Vector2f(draw->sprite.getGlobalBounds().width, draw->sprite.getGlobalBounds().height));
        hitbox->rect.setOutlineColor(sf::Color::Red);
        hitbox->rect.setOutlineThickness(1.0f);

        registry.emplace_component<ecs::EnemyState>(enemy, state);
        registry.emplace_component<ecs::LoopMovement>(enemy, minX, maxX, minY, maxY, speed, angle, radius, angular_speed);
        registry.emplace_component<ecs::EntityType>(enemy, Type::Ennemy);
        registry.emplace_component<ecs::CollisionState>(enemy, false);
        registry.emplace_component<ecs::LifeState>(enemy, true);
        registry.emplace_component<ecs::Shooting>(enemy, 1.5f, 0.5f);
    }

    void CreateEnnemies::create_enemies(Registry& registry, sf::RenderWindow& window)
    {
        int max_y = window.getSize().y - 25;
        int max_x = window.getSize().x;

        for (int i = 0; i < 5; ++i) {
            float yPosition = i * 100.0f;
            create_classic_enemy(registry,
                        max_x + i * 50.0, yPosition,
                        100.0f,
                       "assets/sprites/r-type-enemy.gif",
                        0.0f, max_x,
                        0.0f, max_y,
                        0.0f, 0.0f, 0.0f,
                        State::Patrolling);
        }
        for (int i = 0; i < 3; ++i) {
            float yPosition = i * 100.0f;
            create_shooting_enemy(registry,
                        max_x - 100, yPosition,
                        100.0f,
                       "assets/shooting_enemy.png",
                        0.0f, max_x,
                        0.0f, max_y,
                        0.0f, 0.0f, 0.0f,
                        State::Patrolling);
        }

    }

}