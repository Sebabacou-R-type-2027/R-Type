/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** game
*/

#pragma once

#include <SFML/Graphics.hpp>
#include "registry.hpp"
#include "Create_ennemies.hpp"
#include "Handle_collision.hpp"
#include "system.hpp"

namespace rtype {
    class Game {
        public:
            Game(const std::string& title, unsigned int width, unsigned int height, const std::string& playerTexturePath);
            void run();
        private:
            void initChargeBullet();
            void processEvents();
            void update(sf::Clock& clock);
            void render();

            ecs::CreateEnnemies createEnnemies;
            ecs::HandleCollision handleCollision;
            sf::RenderWindow window;
            ecs::Registry registry;
            ecs::System system;
            sf::Texture playerTexture;
            sf::Sprite playerSprite;
    };
}
