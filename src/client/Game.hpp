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
#include "gameStateManager.hpp"
#include "utils/FPSCounter.hpp"
#include "HandlingBullet.hpp"
#include "factories/button_factory.hpp"

namespace rtype {
    class Game {
        public:
            Game(const std::string& title, unsigned int width, unsigned int height, const std::string& playerTexturePath);
            void run();
        private:
            void initChargeBullet();
            void processEvents(GameStateManager& gameStateManager);
            void update(sf::Clock& clock);
            void render();
            void renderMenu();
            void renderPauseScreen();
            void renderGameOverScreen();
            void renderGame();
            void startGame();
            void createMenuButtons(sf::Font& font);

            // SFML
            sf::RenderWindow window;
            sf::Sprite playerSprite;
            sf::Texture playerTexture;

            // Game State
            GameStateManager gameStateManager;

            // ECS
            ecs::CreateEnnemies createEnnemies;
            ecs::HandlingBullet handlingBullet;
            ecs::HandleCollision handleCollision;
            ecs::Registry registry;
            ecs::System system;

            // FPS Counter
            sf::Text fpsText;
            FPSCounter fpsCounter;
    };
}
