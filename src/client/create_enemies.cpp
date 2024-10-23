/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** create_Enemies
*/

#include "create_enemies.hpp"
#include <nlohmann/json.hpp>
#include <fstream>

namespace ecs {
    void CreateEnemies::create_classic_enemy(Registry& registry,
                                    float x, float y,
                                    float speed,
                                    const std::string& texturePath,
                                    float minX, float maxX,
                                    float minY, float maxY,
                                    float angle, float radius, float angular_speed)
    {
        auto enemy = registry.spawn_entity();
        std::cout << "Creating enemy entity #" << static_cast<std::size_t>(enemy) << std::endl;
        auto &animation = registry.emplace_component<ecs::Animation>(enemy, texturePath, 8, 1, 0.2f);
        float enemyWidth = animation->imageSize.x;
        registry.emplace_component<ecs::Position>(enemy, x - enemyWidth, y);

        auto& hitbox = registry.emplace_component<ecs::Hitbox>(enemy, ecs::ShapeType::Rectangle, false);
        hitbox->rect = sf::RectangleShape(sf::Vector2f(animation->imageSize.x, animation->imageSize.y));
        hitbox->rect.setOutlineColor(sf::Color::Red);
        hitbox->rect.setOutlineThickness(1.0f);

        registry.emplace_component<ecs::LoopMovement>(enemy, minX, maxX, minY, maxY, speed, angle, radius, angular_speed);
        registry.emplace_component<ecs::EntityType>(enemy, Type::Ennemy);
        registry.emplace_component<ecs::CollisionState>(enemy, false);
        registry.emplace_component<ecs::LifeState>(enemy, true);
    }

    void CreateEnemies::create_shooting_enemy(Registry& registry,
                                    float x, float y,
                                    float speed,
                                    const std::string& texturePath,
                                    float minX, float maxX,
                                    float minY, float maxY,
                                    float angle, float radius, float angular_speed)
    {
        auto enemy = registry.spawn_entity();
        std::cout << "Creating enemy entity #" << static_cast<std::size_t>(enemy) << std::endl;
        auto &draw = registry.emplace_component<ecs::Drawable>(enemy, texturePath);
        float enemyWidth = draw->sprite.getGlobalBounds().width;
        registry.emplace_component<ecs::Position>(enemy, x - enemyWidth, y);

        auto& hitbox = registry.emplace_component<ecs::Hitbox>(enemy, ecs::ShapeType::Rectangle, false);
        hitbox->rect = sf::RectangleShape(sf::Vector2f(draw->sprite.getGlobalBounds().width, draw->sprite.getGlobalBounds().height));
        hitbox->rect.setOutlineColor(sf::Color::Red);
        hitbox->rect.setOutlineThickness(1.0f);

        registry.emplace_component<ecs::LoopMovement>(enemy, minX, maxX, minY, maxY, speed, angle, radius, angular_speed);
        registry.emplace_component<ecs::EntityType>(enemy, Type::Ennemy);
        registry.emplace_component<ecs::CollisionState>(enemy, false);
        registry.emplace_component<ecs::LifeState>(enemy, true);
        registry.emplace_component<ecs::Shooting>(enemy, 1.5f, 0.5f);
    }

    void CreateEnemies::create_chasing_enemy(Registry& registry,
                                    float x, float y,
                                    float speed,
                                    const std::string& texturePath,
                                    float minX, float maxX,
                                    float minY, float maxY,
                                    float angle, float radius, float angular_speed)
    {
        auto enemy = registry.spawn_entity();
        std::cout << "Creating enemy entity #" << static_cast<std::size_t>(enemy) << std::endl;
        auto &animation = registry.emplace_component<ecs::Animation>(enemy, texturePath, 3, 1, 0.2f);
        float enemyWidth = animation->imageSize.x;
        registry.emplace_component<ecs::Position>(enemy, x - enemyWidth, y);

        auto& hitbox = registry.emplace_component<ecs::Hitbox>(enemy, ecs::ShapeType::Rectangle, false);
        hitbox->rect = sf::RectangleShape(sf::Vector2f(animation->imageSize.x, animation->imageSize.y));
        hitbox->rect.setOutlineColor(sf::Color::Red);
        hitbox->rect.setOutlineThickness(1.0f);

        registry.emplace_component<ecs::EntityType>(enemy, Type::Ennemy);
        registry.emplace_component<ecs::CollisionState>(enemy, false);
        registry.emplace_component<ecs::LifeState>(enemy, true);
        registry.emplace_component<ecs::Chasing>(enemy, speed);
    }

    void CreateEnemies::create_spawner_enemy(Registry& registry,
                                    float x, float y,
                                    float speed,
                                    const std::string& texturePath,
                                    float minX, float maxX,
                                    float minY, float maxY,
                                    float angle, float radius, float angular_speed,
                                    float cooldown, float lastSpawn, float nbSpawned)
    {
        auto enemy = registry.spawn_entity();
        std::cout << "Creating enemy entity #" << static_cast<std::size_t>(enemy) << std::endl;
        auto &animation = registry.emplace_component<ecs::Animation>(enemy, texturePath, 3, 1, 0.2f);
        float enemyWidth = animation->imageSize.x;
        registry.emplace_component<ecs::Position>(enemy, x - enemyWidth, y);

        auto& hitbox = registry.emplace_component<ecs::Hitbox>(enemy, ecs::ShapeType::Rectangle, false);
        hitbox->rect = sf::RectangleShape(sf::Vector2f(animation->imageSize.x, animation->imageSize.y));
        hitbox->rect.setOutlineColor(sf::Color::Red);
        hitbox->rect.setOutlineThickness(1.0f);

        registry.emplace_component<ecs::EntityType>(enemy, Type::Ennemy);
        registry.emplace_component<ecs::CollisionState>(enemy, false);
        registry.emplace_component<ecs::LifeState>(enemy, true);
        registry.emplace_component<ecs::Spawner>(enemy, cooldown, lastSpawn, nbSpawned);
    }

    void CreateEnemies::create_initial_enemies(Registry& registry, sf::RenderWindow& window)
    {
        int max_y = window.getSize().y - 25;
        int max_x = window.getSize().x;

        // Create Classic Enemies (Sinusoidal Movement, Left to Right)
        // for (int i = 0; i < 5; ++i) {
        //     float yPosition = i * 100.0f;
        //     create_classic_enemy(registry,
        //         max_x, rand() % 600 + 200.0f,    // Starting position x, random y
        //         -100.0f,                          // Horizontal speed (left to right)
        //         "assets/sprites/r-type-enemy.gif",       // Texture
        //         -200.0f, 2000.0f,                // min_x, max_x (movement bounds on x)
        //         200.0f, 800.0f,                  // min_y, max_y (vertical bounds)
        //         0.0f,                            // angle (no initial angle needed)
        //         100.0f,                          // radius for sinusoidal wave
        //         2.0f);
        // }

        // Create Shooting Enemies (Up and Down)
        // for (int i = 0; i < 3; ++i) {
        //     float yPosition = i * 100.0f;
        //     create_shooting_enemy(registry,
        //         max_x - 100, yPosition,          // Starting position x, y
        //         0.0f,                            // Horizontal speed (no horizontal movement)
        //         "assets/shooting_enemy.png",     // Texture
        //         0.0f, max_x,                     // min_x, max_x (not relevant for up/down movement)
        //         200.0f, 800.0f,                  // min_y, max_y (up and down movement range)
        //         0.0f,                            // angle (no need for angle)
        //         0.0f,                            // radius (no sinusoidal movement)
        //         0.0f);                            // angular_speed (no sinusoidal movement)
        // }
        // for (int i = 0; i < 3; ++i) {
        //     float yPosition = i * 100.0f;
        //     create_chasing_enemy(registry,
        //         max_x - 100, yPosition,          // Starting position x, y
        //         1.0f,                            // Horizontal speed (no horizontal movement)
        //         "assets/Chasing enemy/r-typesheet11_right.gif",     // Texture
        //         0.0f, max_x,                     // min_x, max_x (not relevant for up/down movement)
        //         0.0f, max_y,                  // min_y, max_y (up and down movement range)
        //         0.0f,                            // angle (no need for angle)
        //         0.0f,                            // radius (no sinusoidal movement)
        //         0.0f);                            // angular_speed (no sinusoidal movement)
        // }
        for (int i = 0; i < 1; ++i) {
            float yPosition = i * 100.0f;
            create_spawner_enemy(registry,
                max_x - 100, yPosition,          // Starting position x, y
                1.0f,                            // Horizontal speed (no horizontal movement)
                "assets/Chasing_enemy/r-typesheet11_right.gif",     // Texture
                0.0f, max_x,                     // min_x, max_x (not relevant for up/down movement)
                0.0f, max_y,                  // min_y, max_y (up and down movement range)
                0.0f,                            // angle (no need for angle)
                0.0f,                            // radius (no sinusoidal movement)
                0.0f,
                100.0f, 2.0f, 5.0f);                            // angular_speed (no sinusoidal movement)
        }
    }


    void CreateEnemies::loadWavesFromJson(Registry& registry, sf::RenderWindow& window, const std::string& filePath)
    {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filePath << std::endl;
            return;
        }

        nlohmann::json jsonData;
        file >> jsonData;

        if (!jsonData.contains("levels")) {
            std::cerr << "Invalid JSON format: missing 'levels' key" << std::endl;
            return;
        }

        for (const auto& level : jsonData["levels"]) {
            if (!level.contains("waves")) {
                std::cerr << "Invalid JSON format: missing 'waves' key in level" << std::endl;
                continue;
            }

            for (const auto& wave : level["waves"]) {
                if (!wave.contains("mobs")) {
                    continue;
                }

                for (const auto& mob : wave["mobs"]) {
                    std::string type = mob["type"];
                    int wave_id = wave["wave_id"];
                    std::cout << "Wave ID: " << wave_id << std::endl;
                    float x = mob["x"];
                    float y = mob["y"];
                    // x += wave_id * 1920.0f;
                    float speed = 100.0f;
                    std::string texturePath = (type == "enemy1") ? "assets/sprites/r-type-enemy.gif" : "assets/shooting_enemy.png";

                    if (type == "enemy1") {
                        create_classic_enemy(registry, x, y, speed, texturePath, 0.0f, window.getSize().x, 0.0f, window.getSize().y);
                        std::cout << "Creating classic enemy at (" << x << ", " << y << ")" << std::endl;
                    } else if (type == "enemy2") {
                        create_shooting_enemy(registry, x, y, speed, texturePath, 0.0f, window.getSize().x, 0.0f, window.getSize().y);
                    }
                }
            }
        }
    }
}
