/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** create_ennemies
*/

#include <iostream>
#include <chrono>
#include "registry.hpp"

namespace ecs {
    class CreateEnnemies {
        public:
            CreateEnnemies() = default;
            ~CreateEnnemies() = default;
            void create_enemy(Registry& registry,
            float x, float y,
            float speed,
            const std::string& texturePath,
            float minX, float maxX,
            float minY, float maxY,
            float angle = 0.0f, float radius = 0.0f, float angular_speed = 0.0f,
            State state = State::Patrolling);
            void create_enemies(Registry& registry, sf::RenderWindow& window);
    };
}