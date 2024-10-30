/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Create_powerup
*/


#include "Create_powerup.hpp"

namespace ecs {
    void  CreatePowerUp::create_classic_powerup(Registry& registry,
                                    float x, float y,
                                    float speed,
                                    const std::string& texturePath,
                                    float minX, float maxX,
                                    float minY, float maxY,
                                    float angle, float radius, float angular_speed,
                                    State state)
    {
        auto powerup = registry.spawn_entity();
        std::cout << "Creating powerup entity #" << static_cast<std::size_t>(powerup) << std::endl;
        //auto &animation = registry.emplace_component<ecs::Animation>(enemy, texturePath, 8, 1, 0.2f);
        float powerupWidth = animation->imageSize.x;
        registry.emplace_component<ecs::Position>(powerup, x, y);

        auto& hitbox = registry.emplace_component<ecs::Hitbox>(powerup, ecs::ShapeType::Rectangle, false, true);
        hitbox->rect = sf::RectangleShape(sf::Vector2f(animation->imageSize.x, animation->imageSize.y));
        hitbox->rect.setOutlineColor(sf::Color::Red);
        hitbox->rect.setOutlineThickness(1.0f);

        registry.emplace_component<ecs::PowerUpState>(powerup, state);
        registry.emplace_component<ecs::LoopMovement>(powerup, minX, maxX, minY, maxY, speed, angle, radius, angular_speed);
        registry.emplace_component<ecs::EntityType>(powerup, Type::PowerUp);
        registry.emplace_component<ecs::CollisionState>(powerup, false);
        registry.emplace_component<ecs::LifeState>(powerup, true);
    }


    void CreatePowerUp::create_powerup(Registry& registry, sf::RenderWindow& window)
    {
        int max_y = window.getSize().y - 25;
        int max_x = window.getSize().x;

        create_classic_powerup(registry,
                        max_x + i * 10.0, yPosition,
                        100.0f,
                       "assets/Power-Up/power1.png",
                        0.0f, max_x,
                        0.0f, max_y,
                        0.0f, 0.0f, 0.0f,
                        State::Patrolling);
    }

}