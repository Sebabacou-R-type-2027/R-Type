/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** game
*/

#pragma once

#include <SFML/Graphics.hpp>
#include "registry.hpp"
#include "create_ennemies.hpp"
#include "system.hpp"

#include "bullet_event.hpp"

namespace rtype {
    class Game {
        public:
            Game(const std::string& title, unsigned int width, unsigned int height, const std::string& playerTexturePath);
            void run();
        private:
            void initChargeBullet();
            void processEvents();
            void update(rtype::game::BulletSystem& bulletSystem);
            void render();

            ecs::CreateEnnemies createEnnemies;
            sf::RenderWindow window;
            ecs::Registry registry;
            ecs::System system;
            rtype::game::BulletSystem bulletSystem;
            sf::Texture playerTexture;
            sf::Sprite playerSprite;
    };
}
