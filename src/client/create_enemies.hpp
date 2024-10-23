/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** create_Enemies
*/

#pragma once

#include <iostream>
#include <chrono>
#include "registry.hpp"

namespace ecs {
    /**
     * @brief Class that contains all the logic of the creation of Enemies
     */
    class CreateEnemies {
        public:
            CreateEnemies() = default;
            ~CreateEnemies() = default;
            /**
             * @brief Create an ennemy
             *
             * @param registry contains all the logic of the components
             * @param x Position on the x axis
             * @param y Position on the y axis
             * @param speed Speed of the ennemy
             * @param texturePath Path to the texture of the ennemy
             * @param minX Minimum position on the x axis
             * @param maxX Maximum position on the x axis
             * @param minY Minimum position on the y axis
             * @param maxY Maximum position on the y axis
             * @param angle Angle of the ennemy
             * @param radius Radius of the ennemy
             * @param angular_speed Angular speed of the ennemy
             * @param state State of the ennemy
             * @return void
             */
            void create_classic_enemy(Registry& registry,
            float x, float y,
            float speed,
            const std::string& texturePath,
            float minX, float maxX,
            float minY, float maxY,
            float angle = 0.0f, float radius = 0.0f, float angular_speed = 0.0f);

            /**
             * @brief Create a shooting ennemy
             *
             * @param registry contains all the logic of the components
             * @param x Position on the x axis
             * @param y Position on the y axis
             * @param speed Speed of the ennemy
             * @param texturePath Path to the texture of the ennemy
             * @param minX Minimum position on the x axis
             * @param maxX Maximum position on the x axis
             * @param minY Minimum position on the y axis
             * @param maxY Maximum position on the y axis
             * @param angle Angle of the ennemy
             * @param radius Radius of the ennemy
             * @param angular_speed Angular speed of the ennemy
             * @param state State of the ennemy
             * @return void
             */
            void create_shooting_enemy(Registry& registry,
            float x, float y,
            float speed,
            const std::string& texturePath,
            float minX, float maxX,
            float minY, float maxY,
            float angle = 0.0f, float radius = 0.0f, float angular_speed = 0.0f);

            /**
             * @brief Create a chasing ennemy
             *
             * @param registry contains all the logic of the components
             * @param window contains the window of the game
             * @param x Position on the x axis
             * @param y Position on the y axis
             * @param speed Speed of the ennemy
             * @param texturePath Path to the texture of the ennemy
             * @param minX Minimum position on the x axis
             * @param maxX Maximum position on the x axis
             * @param minY Minimum position on the y axis
             * @param maxY Maximum position on the y axis
             * @param angle Angle of the ennemy
             * @param radius Radius of the ennemy
             * @param angular_speed Angular speed of the ennemy
             * @return void
             */
            void create_chasing_enemy(Registry& registry,
            float x, float y,
            float speed,
            const std::string& texturePath,
            float minX, float maxX,
            float minY, float maxY,
            float angle = 0.0f, float radius = 0.0f, float angular_speed = 0.0f);

            /**
             * @brief Create a spawner ennemy
             *
             * @param registry contains all the logic of the components
             * @param x Position on the x axis
             * @param y Position on the y axis
             * @param speed Speed of the ennemy
             * @param texturePath Path to the texture of the ennemy
             * @param minX Minimum position on the x axis
             * @param maxX Maximum position on the x axis
             * @param minY Minimum position on the y axis
             * @param maxY Maximum position on the y axis
             * @param angle Angle of the ennemy
             * @param radius Radius of the ennemy
             * @param angular_speed Angular speed of the ennemy
             * @return void
             */
            void create_spawner_enemy(Registry& registry,
            float x, float y,
            float speed,
            const std::string& texturePath,
            float minX, float maxX,
            float minY, float maxY,
            float angle = 0.0f, float radius = 0.0f, float angular_speed = 0.0f,
            float cooldown = 1.0f, float lastSpawn = 0.0f, float nbSpawned = 0.0f);
            /**
             * @brief Create Enemies
             *
             * @param registry contains all the logic of the components
             * @param window contains the window of the game
             * @return void
             */
            void create_initial_enemies(Registry& registry, sf::RenderWindow& window);

            void loadWavesFromJson(Registry& registry, sf::RenderWindow& window, const std::string& filePath);

    };
}